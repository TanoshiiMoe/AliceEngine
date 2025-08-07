#include "pch.h"
#include "PackageResourceManager.h"
#include "D2DRenderManager.h"
#include <Helpers/FileHelper.h>

PackageResourceManager::PackageResourceManager()
{
	// Image
	// Create WIC factory
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf());

	// 5. WIC Imaging Factory ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicImagingFactory));
	assert(SUCCEEDED(hr));

}

PackageResourceManager::~PackageResourceManager()
{
	// For ImageDraw
	m_wicImagingFactory = nullptr;
}

void PackageResourceManager::Initialize()
{
	std::vector<std::wstring> filesPaths;
	FileHelper::ResourceFilesInBuildPath(L"Resource", filesPaths);
	for (const auto& path : filesPaths)
	{
		if (m_preloadedPaths.find(path) != m_preloadedPaths.end())continue;
		std::pair<std::wstring, std::wstring> nameInfo = FileHelper::ExtractFileNameAndExtension(path);
		std::wstring uuid = path;
		FPacakageInfo info(EResourceSpecifier::Max, nameInfo.first, nameInfo.second, uuid);
		m_preloadedPaths[path] = info;
	}
}

void PackageResourceManager::UnloadData()
{
	for (auto& m_preloadedPath : m_preloadedPaths)
	{
		m_preloadedPath.second.loadCount = 0; // ��� ��Ʈ���� ��ε� ���·� ����
	}
	m_loadedBitmaps.clear();
}

void PackageResourceManager::UnloadData(std::wstring& path)
{
	if (m_preloadedPaths.find(path) != m_preloadedPaths.end())
	{
		m_preloadedPaths[path].loadCount--; //ī��Ʈ 1 ����

		if (m_preloadedPaths[path].loadCount <= 0)
		{
			m_loadedBitmaps.erase(path); // ��� ī��Ʈ�� 0 ������ ��� ĳ�� ����
		}
	}
}

void PackageResourceManager::UnloadDataDir(std::wstring& dirPath)
{
	std::vector<std::wstring> filesPaths;
	FileHelper::ResourceFilesInBuildPath(dirPath, filesPaths);
	for (auto& filesPath : filesPaths)
	{
		UnloadData(filesPath);
	}
}

std::shared_ptr<ID2D1Bitmap1> PackageResourceManager::CreateBitmapFromFile(const wchar_t* path)
{
	// �ش� ��ΰ� �����Ѵٸ�
	std::wstring absolutePath = FileHelper::ToAbsolutePath(path);
	if (m_preloadedPaths.find(absolutePath) != m_preloadedPaths.end())
	{
		if (!m_loadedBitmaps[absolutePath].expired())
		{
			// ĳ�ÿ��� ��Ʈ���� �����ɴϴ�.
			if (m_loadedBitmaps.find(absolutePath) != m_loadedBitmaps.end())
			{
				m_preloadedPaths[absolutePath].loadCount++;
				return m_loadedBitmaps[absolutePath].lock();
			}
			else
			{
				// ������ ����� ���� ������ ��Ʈ���� �ε���� ���� ���
				return nullptr;
				assert(SUCCEEDED(E_FAIL));
			}
		}
		else
		{
			// ó�� �ε��ϴ� ���
			ComPtr<ID2D1Bitmap1> comPtrBitmap;
			HRESULT hr = D2DRenderManager::GetInstance().CreateBitmapFromFile(absolutePath.c_str(), comPtrBitmap.GetAddressOf());
			if (SUCCEEDED(hr)) {
				// Ŀ���� �����ڷ� COM ��ü ����
				auto deleter = [](ID2D1Bitmap1* p) {if (p) p->Release();};
				ID2D1Bitmap1* rawBitmap = comPtrBitmap.Detach(); // ������ ����
				std::shared_ptr<ID2D1Bitmap1> sharedBitmap(rawBitmap, deleter);

				m_preloadedPaths[absolutePath].loadCount++;
				m_loadedBitmaps[m_preloadedPaths[absolutePath].uuid] = sharedBitmap;
				return sharedBitmap;
			}
			else
			{
				assert(FAILED(hr));
			}
		}
	}
	else
	{
		// ��� Ȥ�� ������ �������� ����.
		//assert(SUCCEEDED(E_FAIL));
		return nullptr;
	}
}

std::wstring PackageResourceManager::FormAtBytes(UINT64 bytes)
{
	constexpr double KB = 1024.0;
	constexpr double MB = KB * 1024.0;
	constexpr double GB = MB * 1024.0;

	std::wostringstream oss;
	oss << std::fixed << std::setprecision(2);

	if (bytes >= static_cast<UINT64>(GB))
		oss << (bytes / GB) << L" GB";
	else if (bytes >= static_cast<UINT64>(MB))
		oss << (bytes / MB) << L" MB";
	else if (bytes >= static_cast<UINT64>(KB))
		oss << (bytes / KB) << L" KB";
	else
		oss << bytes << " B";

	return oss.str();
}

FMemoryInfo PackageResourceManager::GetMemoryInfo()
{
	DXGI_QUERY_VIDEO_MEMORY_INFO memInfo = {};
	D2DRenderManager::GetInstance().m_dxgiAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);

	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	// ���� ���μ����� �޸� ��� ���� ��ȸ
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	FMemoryInfo info;
	info.VRAMUssage = FormAtBytes(memInfo.CurrentUsage);
	info.DRAMUssage = FormAtBytes(pmc.WorkingSetSize);
	info.PageFile = FormAtBytes(pmc.PagefileUsage);
	return info;
}

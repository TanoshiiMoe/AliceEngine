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

	// 5. WIC Imaging Factory 생성
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
		std::wstring uuid = path + std::to_wstring(FRandom::GetRandomInRange(0, 10000));
		FPacakageInfo info(EResourceSpecifier::Max, nameInfo.first, nameInfo.second, uuid);
		m_preloadedPaths[path] = info;
	}
}

void PackageResourceManager::UnloadData()
{
	m_loadedBitmaps.clear();
}

std::weak_ptr<ID2D1Bitmap1> PackageResourceManager::CreateBitmapFromFile(const wchar_t* path)
{
	// 해당 경로가 존재한다면
	std::wstring absolutePath = FileHelper::ToAbsolutePath(path);
	if (m_preloadedPaths.find(absolutePath) != m_preloadedPaths.end())
	{
		if (m_preloadedPaths[absolutePath].bIsLoaded)
		{
			// 캐시에서 비트맵을 가져옵니다.
			auto it = m_loadedBitmaps.find(m_preloadedPaths[absolutePath].uuid);
			if (it != m_loadedBitmaps.end())
			{
				return it->second;
			}
			else
			{
				return std::weak_ptr<ID2D1Bitmap1>();
				// 기존에 사용한 적은 있지만 비트맵이 로드되지 않은 경우
				assert(SUCCEEDED(E_FAIL));
			}
		}
		else
		{
			ComPtr<IWICBitmapDecoder>     decoder;
			ComPtr<IWICBitmapFrameDecode> frame;
			ComPtr<IWICFormatConverter>   converter;

			// ① 디코더 생성
			HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
				absolutePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
			//assert(FAILED(hr));

			// ② 첫 프레임 얻기
			hr = decoder->GetFrame(0, &frame);
			//assert(FAILED(hr));

			// ③ 포맷 변환기 생성
			hr = m_wicImagingFactory->CreateFormatConverter(&converter);
			//assert(FAILED(hr));

			// ④ GUID_WICPixelFormat32bppPBGRA로 변환
			hr = converter->Initialize(
				frame.Get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.0f,
				WICBitmapPaletteTypeCustom
			);
			//assert(FAILED(hr));

			// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
			D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_NONE,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);

			// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
			ComPtr<ID2D1Bitmap1> comPtrBitmap;
			hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, &comPtrBitmap);

			if (SUCCEEDED(hr)) {
				// 커스텀 삭제자로 COM 객체 래핑
				auto deleter = [](ID2D1Bitmap1* p) {
					if (p) p->Release();
					};

				ID2D1Bitmap1* rawBitmap = comPtrBitmap.Detach(); // 소유권 이전
				std::shared_ptr<ID2D1Bitmap1> sharedBitmap(rawBitmap, deleter);

				m_preloadedPaths[absolutePath].bIsLoaded = true; // 비트맵이 로드되었음을 표시
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
		// 경로 혹은 파일이 존재하지 않음.
		//assert(SUCCEEDED(E_FAIL));
		return std::weak_ptr<ID2D1Bitmap1>();
	}
	return std::weak_ptr<ID2D1Bitmap1>();
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

	// 현재 프로세스의 메모리 사용 정보 조회
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	FMemoryInfo info;
	info.VRAMUssage = FormAtBytes(memInfo.CurrentUsage);
	info.DRAMUssage = FormAtBytes(pmc.WorkingSetSize);
	info.PageFile = FormAtBytes(pmc.PagefileUsage);
	return info;
}

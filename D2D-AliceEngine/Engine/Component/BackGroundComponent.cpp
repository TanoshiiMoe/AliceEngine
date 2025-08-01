#include "pch.h"
#include "BackGroundComponent.h"
#include "Manager/D2DRenderManager.h"
#include "Application.h"
#include <Helpers/FFmpegHelper.h>
#include <Component/TransformComponent.h>
#include <Component/RenderComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/TimerManager.h>
#include <System/RenderSystem.h>
#include <Object/gameObject.h>
#include <Object/Camera.h>

BackGroundComponent::BackGroundComponent()
{
}

BackGroundComponent::~BackGroundComponent()
{
	files.clear();
	for (auto& m_bitmap : m_bitmaps)
	{
		m_bitmap.reset();
	}
	m_bitmaps.clear();
	PackageResourceManager::GetInstance().UnloadDataDir(fileDirPath); // ���� ���� ��ε�
}

void BackGroundComponent::Initialize()
{
	__super::Initialize();
	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);
}

void BackGroundComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	ReleaseFrames();
	UpdateFrames(deltaSeconds);
}

void BackGroundComponent::UpdateFrames(const float& deltaSeconds)
{
	if (bPlay)	// �÷��� ������ ���� �÷���
	{
		m_fFPSLastTime += deltaSeconds;
		if (m_fFPSLastTime >= m_fFPSTime)	// 1/60 �ʿ� �� ����
		{
			if (m_maxClipSize != 0)
			{
				if (m_curClip + 1 >= m_maxClipSize)
				{
					PackageResourceManager::GetInstance().UnloadDataDir(fileDirPath);
					m_bitmaps.clear();
				}
				m_curClip = (m_curClip + 1) % m_maxClipSize;
			}
			m_fFPSLastTime = 0;
		}
	}
}

void BackGroundComponent::ReleaseFrames()
{
	// ���� ������ �ֺ��� ĳ�� ����
	size_t start = (m_curClip > cacheSize) ? m_curClip - cacheSize : 0;
	size_t end = min(m_bitmaps.size(), m_curClip + cacheSize);

	for (size_t i = 0; i < m_bitmaps.size(); ++i) {
		if (i < start || i > end) {
			PackageResourceManager::GetInstance().UnloadData(files[i]); // �ش� ������ ��ε�
			m_bitmaps[i].reset(); // ������� �ʴ� ������ ����
		}
	}
}

void BackGroundComponent::LoadData(const std::wstring& path, const int& fps, const std::wstring& extension, const int& quality, const bool& bIsBackGround)
{
	fileDirPath = FileHelper::get_folder_path(Define::BASE_RESOURCE_PATH + path); // ���� ���� ��� ����
	files = FFmpegHelper::GetFramesFromVideo(Define::BASE_RESOURCE_PATH + path, 12, extension, static_cast<float>(quality), bIsBackGround);
	if (files.empty()) return;

	m_fFPSTime = 1.0f / fps;
	m_maxClipSize = files.size();
	m_curClip = 0;
	m_bitmaps.clear();
}

void BackGroundComponent::LoadFromFolder(const std::wstring& folderPath, int fps, const std::wstring& extension)
{
	bImage = true;
	imageExtension = extension;
	std::wstring pattern = std::wstring(L"frames\\*.") + extension;
	fileDirPath = Define::BASE_RESOURCE_PATH + folderPath;
	files = FileHelper::GetFilesWithPattern(FileHelper::GetProjectRootPath() + L"\\" + fileDirPath, pattern);
	if (files.empty()) return;

	m_fFPSTime = 1.0f / fps;
	m_maxClipSize = files.size();
	m_curClip = files.size()-1;
	m_bitmaps.clear();
	m_bitmaps.resize(m_maxClipSize);

	for (size_t i = 0; i < files.size(); ++i)
	{
		std::pair<std::wstring, std::wstring> path = FileHelper::ExtractFileNameAndExtension(files[i]);
		std::wstring folderPath = path.first; std::wstring fileName = path.second;
		std::wstring parentDir = folderPath + L"\\frames\\" + fileName + L"." + extension; // ���� ��� ���� ��� ����

		// �����ϰ� ��Ʈ�� �н�
		auto bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(parentDir.c_str());

		if (bitmap)
			m_bitmaps[i] = bitmap;
		else
			OutputDebugStringW(L"Fail to load bitmap");
	}
}

void BackGroundComponent::LoadFrame(size_t frameIndex)
{
	std::shared_ptr<ID2D1Bitmap1> temp = PackageResourceManager::GetInstance().CreateBitmapFromFile(files[frameIndex].c_str());
	if (m_bitmaps.size() <= frameIndex)
	{
		m_bitmaps.resize(frameIndex + 1);
	}
	m_bitmaps[frameIndex] = temp;
}

void BackGroundComponent::LoadFrameFromFolder(size_t frameIndex)
{
	std::pair<std::wstring, std::wstring> path = FileHelper::ExtractFileNameAndExtension(files[frameIndex]);
	std::wstring folderPath = path.first; std::wstring fileName = path.second;
	std::wstring parentDir = folderPath + L"\\frames\\" + fileName + L"." + imageExtension; // ���� ��� ���� ��� ����

	std::shared_ptr<ID2D1Bitmap1> temp = PackageResourceManager::GetInstance().CreateBitmapFromFile(parentDir.c_str());
	if (m_bitmaps.size() <= frameIndex)
	{
		m_bitmaps.resize(frameIndex + 1);
	}
	m_bitmaps[frameIndex] = temp;
}

void BackGroundComponent::Release()
{
	__super::Release();
}

void BackGroundComponent::Render()
{
	if (files.empty()) return;
	if (m_bitmaps.size() <= m_curClip)
	{
		// �ʿ��� �����Ӹ� �ε�
		if (!bImage)
			LoadFrame(m_curClip);
		else
			LoadFrameFromFolder(m_curClip);
	}
	if (m_bitmaps.empty()) return;
	if (m_bitmaps[m_curClip] == nullptr) return;
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context) return;

	__super::Render();

	D2D1::Matrix3x2F backToD2DTransform = D2D1::Matrix3x2F::Translation(-GetBitmapSizeX() / 2, -GetBitmapSizeY() / 2);
	D2D1::Matrix3x2F pivotTransform = D2D1::Matrix3x2F::Translation((GetBitmapSizeX() / 2) * (GetPivot()->x - 0.5f), (GetBitmapSizeY() / 2) * (GetPivot()->y - 0.5f));
	context->SetTransform(backToD2DTransform * pivotTransform * view);

	// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	context->SetTransform(view);
	context->DrawBitmap(m_bitmaps[m_curClip].get());
}

float BackGroundComponent::GetBitmapSizeX()
{
	if (m_bitmaps.size() > m_curClip && GetOwner())
	{
		return static_cast<float>(bmpSize.width) * owner->transform()->GetScale().x;
	}
	return 0;
}

float BackGroundComponent::GetBitmapSizeY()
{
	if (m_bitmaps.size() > m_curClip && GetOwner())
	{
		return static_cast<float>(bmpSize.height) * owner->transform()->GetScale().y;
	}
	return 0;
}

FVector2 BackGroundComponent::GetSize()
{
	if (m_bitmaps.size() > m_curClip && GetOwner())
	{
		if (m_bitmaps[m_curClip])
		{
			return FVector2(static_cast<float>(bmpSize.width) * owner->transform()->GetScale().x, static_cast<float>(bmpSize.height) * owner->transform()->GetScale().y);
		}
	}
	return FVector2();
}

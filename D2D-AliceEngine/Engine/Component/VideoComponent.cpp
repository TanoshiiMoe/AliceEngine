#include "pch.h"
#include "VideoComponent.h"
#include "SpriteRenderer.h"
#include "Manager/D2DRenderManager.h"
#include "Application.h"
#include <Helpers/FFmpegHelper.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/RenderComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/TimerManager.h>
#include <System/RenderSystem.h>
#include <Object/gameObject.h>
#include <Object/Camera.h>

VideoComponent::~VideoComponent()
{
	files.clear();
	m_bitmaps.clear();
	PackageResourceManager::GetInstance().UnloadDataDir(fileDirPath); // 비디오 파일 언로드
}

void VideoComponent::Initialize()
{
	__super::Initialize();
	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = weak_from_this()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);
}

void VideoComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	ReleaseFrames();
	UpdateFrames();
}

void VideoComponent::UpdateFrames()
{
	if (bPlay)	// 플레이 가능할 때만 플레이
	{
		m_fFPSLastTime = TimerManager::GetInstance().GetTotalTime() - m_fcountOneSecond;
		if (m_fFPSLastTime >= m_fFPSTime)	// 1/60 초에 한 번씩
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
			m_fcountOneSecond = TimerManager::GetInstance().GetTotalTime();
		}
	}
}

void VideoComponent::ReleaseFrames()
{
	// 현재 프레임 주변만 캐시 유지
	size_t start = (m_curClip > cacheSize) ? m_curClip - cacheSize : 0;
	size_t end = min(m_bitmaps.size(), m_curClip + cacheSize);

	for (size_t i = 0; i < m_bitmaps.size(); ++i) {
		if (i < start || i > end) {
			PackageResourceManager::GetInstance().UnloadData(files[i]); // 해당 프레임 언로드
			m_bitmaps[i].reset(); // 사용하지 않는 프레임 해제
		}
	}
}

void VideoComponent::LoadData(const std::wstring& path, const int& fps, const std::wstring& extension, const int& quality)
{
	fileDirPath = FileHelper::get_folder_path(Define::BASE_RESOURCE_PATH + path); // 비디오 파일 경로 저장
	files = FFmpegHelper::GetFramesFromVideo(Define::BASE_RESOURCE_PATH + path, 12, extension, quality);
	if (files.empty()) return;

	m_fFPSTime = 1.0f / fps;
	m_maxClipSize = files.size();
	m_curClip = 0;
	m_bitmaps.clear();
}

void VideoComponent::LoadFrame(size_t frameIndex)
{
	std::shared_ptr<ID2D1Bitmap1> temp = PackageResourceManager::GetInstance().CreateBitmapFromFile(files[frameIndex].c_str());
	if (m_bitmaps.size() <= frameIndex)
	{
		m_bitmaps.resize(frameIndex + 1);
	}
	m_bitmaps[frameIndex] = temp;
}

void VideoComponent::Release()
{
	__super::Release();
}

/*
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
*/

void VideoComponent::Render()
{
	__super::Render();
	if (files.empty()) return;
	if (m_bitmaps.size() <= m_curClip) 
	{
		// 필요한 프레임만 로딩
		LoadFrame(m_curClip);
	}
	if (m_bitmaps.empty()) return;
	if (m_bitmaps[m_curClip] == nullptr) return;

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmaps[m_curClip]->GetPixelSize(); // 비트맵 크기 및 피벗
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * GetPivot()->x,
		bmpSize.height * GetPivot()->y
	};
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = GetTransform()->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// 로컬 피벗 기준 월드 변환, 카메라 역행렬 적용
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity 좌표계면 변환 추가
	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// 최종 변환 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	context->SetTransform(view);
	context->DrawBitmap(m_bitmaps[m_curClip].get());
}

FVector2 VideoComponent::GetSize()
{
	if (m_bitmaps.empty() == false)
	{
		ComPtr<ID2D1Bitmap1> bitmapStrong;
		if (m_bitmaps[0])
		{
			D2D1_SIZE_U bmpSize = m_bitmaps[0]->GetPixelSize();
			return FVector2(bmpSize.width, bmpSize.height);
		}
		return FVector2(0, 0);
	}
	return FVector2();
}

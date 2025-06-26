#include "pch.h"
#include "AnimationComponent.h"
#include "SpriteRenderer.h"
#include "Manager/D2DRenderManager.h"
#include "Application.h"
#include <Helpers/FFmpegHelper.h>
#include <Core/Time.h>

void AnimationComponent::Initialize()
{
	__super::Initialize();
}

void AnimationComponent::Update()
{
	__super::Update();

	//ReleaseFrames();
	UpdateFrames();
}

void AnimationComponent::UpdateFrames()
{
	if (bPlay)	// 플레이 가능할 때만 플레이
	{
		m_fFPSLastTime = Time::GetTotalTime() - m_fcountOneSecond;
		if (m_fFPSLastTime >= m_fFPSTime)	// 1/60 초에 한 번씩
		{
			if (m_maxClipSize != 0)
			{
				//if (m_curClip + 1 >= m_maxClipSize)
				//{
				//	m_bitmaps.clear();
				//}
				m_curClip = (m_curClip + 1) % m_maxClipSize;
			}
			m_fcountOneSecond = Time::GetTotalTime();
		}
	}
}

void AnimationComponent::ReleaseFrames()
{
	// 현재 프레임 주변만 캐시 유지
	size_t start = (m_curClip > cacheSize) ? m_curClip - cacheSize : 0;
	size_t end = min(m_bitmaps.size(), m_curClip + cacheSize);

	for (size_t i = 0; i < m_bitmaps.size(); ++i) {
		if (i < start || i > end) {
			if (m_bitmaps[i].lock())
			{
				//m_bitmaps[i].reset();
			}
			//HRESULT hr = m_bitmaps[i]->lock();
			//if (SUCCEEDED(hr) && bitmapStrong) {
			//	// bitmapStrong을 안전하게 사용
			//	m_bitmaps[i]->Reset(); // 사용하지 않는 프레임 해제
			//}
			//else {
			//	// 이미 해제함 (nullptr)
			//}
		}
	}
}

void AnimationComponent::LoadData(const std::wstring& path, const int& fps)
{
	files = FFmpegHelper::GetFramesFromVideo(L"Resource\\" + path, 8);
	if (files.empty()) return;

	m_fFPSTime = 1.0f / fps;
	m_maxClipSize = files.size();
	m_curClip = 0;
	m_bitmaps.clear();
	m_bitmaps.resize(m_maxClipSize + 1);
	for (size_t i = 0 ; i < m_maxClipSize; i++)
	{
		m_bitmaps[i] = PackageResourceManager::Get().CreateBitmapFromFile(files[i].c_str());
	}
}

void AnimationComponent::LoadFrame(size_t frameIndex) {
	//if (m_bitmaps.size() <= frameIndex) 
	//{
	//	m_bitmaps.resize(frameIndex + 1);
	//}
	m_bitmaps[frameIndex] = PackageResourceManager::Get().CreateBitmapFromFile(files[frameIndex].c_str());
}

void AnimationComponent::Release()
{
	__super::Release();
}

/*
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
*/

void AnimationComponent::Render()
{
	if (files.empty()) return;
	//if (m_bitmaps.size() <= m_curClip) 
	//{
	//	// 필요한 프레임만 로딩
	//	LoadFrame(m_curClip);
	//}
	if (m_bitmaps.empty()) return;
	//if (m_bitmaps[m_curClip].lock() == nullptr)
	//{
	//	// 현재 프레임이 로드되지 않은 경우
	//	LoadFrame(m_curClip);
	//	return;
	//}

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmaps[m_curClip].lock()->GetPixelSize(); // 비트맵 크기 및 피벗
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * m_pivot->x,
		bmpSize.height * m_pivot->y
	};
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = m_pTransform->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::Get().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// 로컬 피벗 기준 월드 변환, 카메라 역행렬 적용
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity 좌표계면 변환 추가
	if (D2DRenderManager::Get().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// 최종 변환 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	context->SetTransform(view);
	context->DrawBitmap(m_bitmaps[m_curClip].lock().get());
}

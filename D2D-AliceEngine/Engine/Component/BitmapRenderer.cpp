#include "pch.h"
#include "BitmapRenderer.h"
#include "Manager/D2DRenderManager.h"
#include "Application.h"

void BitmapRenderer::Initialize()
{
	__super::Initialize();
}

void BitmapRenderer::LoadData(const std::wstring& path)
{
	HRESULT hr = D2DRenderManager::Get().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));
}

void BitmapRenderer::Release()
{
	__super::Release();
}

void BitmapRenderer::Render()
{
	if (!m_bitmap)
		return;

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize(); // 비트맵 크기 및 피벗
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
	context->DrawBitmap(m_bitmap.Get());
	context->DrawRectangle(
		D2D1::RectF(0, 0,
			bmpSize.width, bmpSize.height), D2DRenderManager::Get().m_pRedBrush.Get(), 5.0f);
}

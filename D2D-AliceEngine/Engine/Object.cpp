#include "pch.h"
#include "Object.h"
#include "Application.h"

void Object::Initialize()
{
	if (m_localTransform == nullptr)
	{
		m_localTransform = std::make_shared<Transform>();
	}
	if (m_worldTransform == nullptr)
	{
		m_worldTransform = std::make_shared<Transform>();
	}
}

void Object::LoadBitmapData(const std::wstring& path)
{
	HRESULT hr = Application::GetInstance()->m_pD2DRenderer.get()->CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));
}

void Object::Release()
{
	m_localTransform.reset();
	m_worldTransform.reset();
	m_localTransform = nullptr;
	m_worldTransform = nullptr;
	//m_bitmap = nullptr;
	for (auto child : children)
	{
		child->parent.reset();
	}
}

void Object::UpdateWorldTransform()
{
	D2D1::Matrix3x2F mat;

	if (parent.lock())
	{
		mat = m_localTransform->ToMatrix() * parent.lock()->m_worldTransform->ToMatrix();
	}
	else
	{
		mat = m_localTransform->ToMatrix();
	}

	m_worldTransform->SetFromMatrix(mat);

	for (auto child : children)
		child->UpdateWorldTransform();
}

void Object::Render()
{
	ID2D1Bitmap1* bitmap = m_bitmap.Get();
	D2D1_SIZE_U bmpSize = m_bitmap.Get()->GetPixelSize();
	D2D1_POINT_2F worldPivot = {
		bmpSize.width * 0.5f,
		bmpSize.height * 0.5f
	};

	D2D1_RECT_F SrcRect{ 0, 0, bmpSize.width, bmpSize.height }; // 화면 영역, 비트맵 영역
		
	D2D1_POINT_2F localPivot = {
		bmpSize.width * m_pivot.x,
		bmpSize.height * m_pivot.y
	};
	// 최종 월드 변환 행렬
	D2D1::Matrix3x2F worldMatrix = m_worldTransform->ToMatrix(localPivot);

	// 월드에서 이미지 중심이 어디에 찍히는지 계산
	D2D1_POINT_2F worldCenter = worldMatrix.TransformPoint(worldPivot);

	// 해당 중심 기준으로 사각형 보정
	D2D1_RECT_F DestRect = D2D1::RectF(
		worldCenter.x - worldPivot.x,
		worldCenter.y - worldPivot.y,
		worldCenter.x + worldPivot.x,
		worldCenter.y + worldPivot.y
	);

	Camera* camera = Application::GetInstance()->m_mainCamera.get();
	D2D1::Matrix3x2F cameraInverse = camera->m_worldTransform->ToMatrix();
	cameraInverse.Invert();

	// 최종 변환: 월드 * 카메라 역행렬
	D2D1::Matrix3x2F transform = m_worldTransform->ToMatrix(worldCenter); // 변환 행렬
	D2D1::Matrix3x2F finalTransform = transform * cameraInverse;

	if (Application::GetInstance()->m_pD2DRenderer->m_eTransformType == ETransformType::Unity)
	{
		float centerX = Define::SCREEN_WIDTH * 0.5f;
		float centerY = Define::SCREEN_HEIGHT * 0.5f;
		// Unity 스타일 변환: 화면 중심을 기준으로 변환

		D2D1::Matrix3x2F unityMat = D2D1::Matrix3x2F(
			1, 0,
			0, -1,
			centerX, centerY
		);
		// 4. 최종 월드 변환
		finalTransform = finalTransform * unityMat;
	}

	ID2D1DeviceContext7* m_d2dDeviceContext = Application::GetInstance()->m_pD2DRenderer.get()->m_d2dDeviceContext.Get();
	m_d2dDeviceContext->SetTransform(finalTransform);
	m_d2dDeviceContext->DrawBitmap(bitmap, DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
}

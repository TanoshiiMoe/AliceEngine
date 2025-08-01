#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <Object/Camera.h>
#include <Manager/SceneManager.h>
#include <Component/Rigidbody2D.h>
#include <Component/TransformComponent.h>

RenderComponent::RenderComponent()
{
	view = D2D1::Matrix3x2F::Identity();
}

RenderComponent::~RenderComponent()
{
	RenderSystem::GetInstance().UnRegist(WeakFromThis<RenderComponent>());
}

void RenderComponent::Initialize()
{
	RenderSystem::GetInstance().Regist(WeakFromThis<RenderComponent>());
}

void RenderComponent::Update()
{
}

void RenderComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void RenderComponent::Release()
{
}

void RenderComponent::Render()
{
	__super::Render();
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (context == nullptr) return;
	Camera* camera = SceneManager::GetCamera();
	view = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F world = GetTransform() ? GetTransform()->ToMatrix() : D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (drawType == Define::EDrawType::WorldSpace)
	{
		view = view * unity;
		view = view * world; // 로컬 피벗 기준 월드 변환, 카메라 역행렬 적용
		cameraInv.Invert();
		view = view * cameraInv;
		// Unity 좌표계면 변환 추가
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}
	else if (drawType == Define::EDrawType::ScreenSpace)
	{
		view = view * world;
	}

	if (bFlip)
	{
		view = D2D1::Matrix3x2F::Scale(-1.f, 1.f) * view;
	}

	view = D2D1::Matrix3x2F::Scale(scale.x, scale.y) * view;

	context->SetTransform(view);
}
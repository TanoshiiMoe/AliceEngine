#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <Object/Camera.h>
#include <Manager/SceneManager.h>
#include <Component/Rigidbody2D.h>

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
	if (!bReadyForRender)
	{
		bReadyForRender = true;
		return;
	}

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (context == nullptr) return;
	Camera* camera = SceneManager::GetCamera();
	view = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F world = D2D1::Matrix3x2F::Identity();
	GetWorldTransform(world);

	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// 로컬 피벗 기준 월드 변환, 카메라 역행렬 적용
	view = view * world;
	
	if (drawType == Define::EDrawType::WorldSpace)
	{
		cameraInv.Invert();
		view = view * cameraInv;
	}

	// Unity 좌표계면 변환 추가
	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}
	if (bFlip)
	{
		view = D2D1::Matrix3x2F::Scale(-1.f, 1.f) * view;
	}

	view = D2D1::Matrix3x2F::Scale(scale.x, scale.y) * view;

	if(drawType == Define::EDrawType::WorldSpace)
		context->SetTransform(view);
}

D2D1::Matrix3x2F RenderComponent::GetWorldTransform(D2D1::Matrix3x2F& world)
{
	if (auto rb = GetOwner()->GetComponent<Rigidbody2D>())
	{
		// RigidBody2D가 있으면 보간된 위치 사용
		FVector2 interpPos = rb->GetInterpolatedPosition();
		float interpRot = rb->GetInterpolatedRotation();
		D2D1_VECTOR_2F scale = GetTransform() ? GetTransform()->GetScale() : D2D1_VECTOR_2F{ 1.0f,1.0f };

		world =
			D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
			D2D1::Matrix3x2F::Rotation(interpRot * 180.0f / Define::PI) *
			D2D1::Matrix3x2F::Translation(interpPos.x, interpPos.y);
	}
	else
	{
		if (drawType == Define::EDrawType::WorldSpace)
		{
			world = GetTransform() ? GetTransform()->ToMatrix() : D2D1::Matrix3x2F::Identity();
		}
		else if (drawType == Define::EDrawType::ScreenSpace)
		{
			world = GetCanvasTransform() ? GetCanvasTransform()->ToMatrix() : D2D1::Matrix3x2F::Identity();
		}
	}
	return world;
}


#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <Object/Camera.h>
#include <Manager/SceneManager.h>
#include <Component/Rigidbody2D.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

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
	relativeTransform.Initialize();
	if (owner) owner->transform()->AddChildObject(&relativeTransform);
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

void RenderComponent::SetDrawType(const Define::EDrawType& type)
{
	drawType = type;
}

void RenderComponent::Render()
{
	__super::Render();
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	if (!context || !camera) return;
	view = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F world = GetOwnerTransform() ? relativeTransform.m_worldTransform.ToMatrix() : D2D1::Matrix3x2F::Identity();

	if (drawType == Define::EDrawType::WorldSpace)
	{
		D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();
		cameraInv.Invert();
		view = view * unity * world * cameraInv;
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

	context->SetTransform(view);
}

void RenderComponent::SetRelativePosition(const FVector2& _pos)
{
	relativeTransform.SetPosition(_pos.x, _pos.y);
}

void RenderComponent::SetRelativeRotation(const float& _rotation)
{
	relativeTransform.SetRotation(_rotation);
}

void RenderComponent::SetRelativeScale(const FVector2& _scale)
{
	relativeTransform.SetScale(_scale.x, _scale.y);
}

FVector2 RenderComponent::GetRelativePosition() const
{
	//D2D1_VECTOR_2F pos = relativeTransform.GetPosition();
	FVector2 pos = relativeTransform.GetPosition();
	return FVector2(pos.x, pos.y);
}

float RenderComponent::GetRelativeRotation()
{
	return relativeTransform.GetRotation();
}

FVector2 RenderComponent::GetRelativeScale()
{
	//D2D1_VECTOR_2F scale = relativeTransform.GetScale();
	FVector2 scale = relativeTransform.GetScale();
	return FVector2(scale.x, scale.y);
}

FVector2 RenderComponent::GetRelativeSize()
{
	// 상대 Transform의 Scale
	FVector2 relativeScale = FVector2(
		relativeTransform.GetScale().x,
		relativeTransform.GetScale().y
	);

	// 최종 크기 = 기본 크기 * 부모 Scale * 상대 Scale
	return FVector2(
		relativeScale.x,
		relativeScale.y
	);
}
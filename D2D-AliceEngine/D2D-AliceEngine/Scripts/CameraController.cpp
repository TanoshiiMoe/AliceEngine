#include "CameraController.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/VideoComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <System/ScriptSystem.h>

void CameraController::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);
}

void CameraController::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void CameraController::OnStart()
{
	m_camera = GetOwner();
	m_camera->AddComponent<InputComponent>()->SetAction(m_camera, [this]() { Input(); });
}

void CameraController::OnEnd()
{

}

void CameraController::Input()
{
	if(Input::IsKeyDown(VK_RIGHT))
	{
		SceneManager::GetCamera()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		SceneManager::GetCamera()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		SceneManager::GetCamera()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		SceneManager::GetCamera()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyPressed(VK_1))
	{
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::D2D);
	}
	if (Input::IsKeyPressed(VK_2))
	{
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::Unity);
	}
}

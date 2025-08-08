#include "SelectScene.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include <Helpers/CoordHelper.h>
#include <Scripts/Widget/SelectWidgetScript.h>

void SelectScene::Initialize()
{
	__super::Initialize();
}

void SelectScene::Release()
{
	__super::Release();
}

void SelectScene::Update()
{
	__super::Update();
}

void SelectScene::OnEnter()
{
	__super::OnEnter();

	m_UI = NewObject<gameObject>(L"UI");
	m_UI->AddComponent<SelectWidgetScript>();

	m_UI->AddComponent<InputComponent>()->SetAction(m_UI->GetHandle(), [this]() {Input(); });
}

void SelectScene::OnExit()
{
}

void SelectScene::Input()
{
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"TitleScene");
	}
}

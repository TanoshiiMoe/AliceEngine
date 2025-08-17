#include "SpineDemoScene.h"
#include <Manager/SceneManager.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/SpineRenderer.h>
#include <Scripts/Spine2D/SpineScript.h>

void SpineDemoScene::Initialize()
{
	__super::Initialize();
}

void SpineDemoScene::Release()
{
	__super::Release();
}

void SpineDemoScene::Update()
{
	__super::Update();
}

void SpineDemoScene::OnEnter()
{
	__super::OnEnter();
	spineObject = NewObject<gameObject>(L"spineObject");
	SpineScript* spine = spineObject->AddComponent<SpineScript>();
	//spine->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
}

void SpineDemoScene::OnExit()
{
	__super::OnExit();
}

void SpineDemoScene::PlayerInput()
{
}

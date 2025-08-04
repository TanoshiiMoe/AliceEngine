#include "SpineScene.h"
#include <Manager/SceneManager.h>
#include "Scripts/TitleUIScript.h"
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Scripts/Spine2D/SpineScript.h>

void SpineScene::Initialize()
{
	__super::Initialize();
}

void SpineScene::Release()
{
	__super::Release();
}

void SpineScene::Update()
{
	__super::Update();
}

void SpineScene::OnEnter()
{
	__super::OnEnter();
	//m_cameraController = NewObject<gameObject>(L"Camera");
	spineObject = NewObject<gameObject>(L"spineObject");
	spineObject->AddComponent<SpineScript>();

}

void SpineScene::OnExit()
{
	__super::OnExit();
}

void SpineScene::PlayerInput()
{
}

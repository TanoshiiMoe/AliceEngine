#include "TitleScene.h"
#include <Manager/SceneManager.h>
#include "Scripts/TitleUIScript.h"

void TitleScene::Initialize()
{
	__super::Initialize();
}

void TitleScene::Release()
{
	__super::Release();
}

void TitleScene::Update()
{
	__super::Update();
}

void TitleScene::OnEnter()
{
	__super::OnEnter();
	m_cameraController = NewObject<gameObject>(L"Camera");

	m_bg = NewObject<gameObject>(L"BackGround");

	m_UI = NewObject<gameObject>(L"UI");
	m_UI->AddComponent<TitleUIScript>();
}

void TitleScene::OnExit()
{
	__super::OnExit();
}

void TitleScene::PlayerInput()
{
}

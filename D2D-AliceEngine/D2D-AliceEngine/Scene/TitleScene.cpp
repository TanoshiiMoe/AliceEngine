#include "TitleScene.h"
#include <Manager/SceneManager.h>
#include "Scripts/TitleUIScript.h"
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/AudioComponent.h>
#include <Scripts/TitleAudioScript.h>
#include <Core/Input.h>
#include <Component/InputComponent.h>

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

	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<TitleAudioScript>();

	m_UI = NewObject<gameObject>(L"UI");
	//m_UI->AddComponent<TitleUIScript>();
	m_UI->AddComponent<TitleWidgetScript>();
}

void TitleScene::OnExit()
{
	__super::OnExit();
}

void TitleScene::PlayerInput()
{
}
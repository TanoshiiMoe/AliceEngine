#include "TitleScene.h"
#include <Manager/SceneManager.h>
#include "Scripts/TitleUIScript.h"
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/AudioComponent.h>
#include <Scripts/TitleAudioScript.h>
#include <Core/Input.h>
#include <Component/InputComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>

void TitleScene::Initialize()
{
	__super::Initialize();

    GamePlayManager& GPM = GamePlayManager::GetInstance();
	GPM.StartGame();
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

	// 테스트용 컷씬 위젯. 이걸 켜서 확인할 것.
	//m_UI->AddComponent<CutSceneWidgetScript>();


	// 디버그용 씬 전환
	gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
	sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_3)) {
			SceneManager::ChangeScene(L"KangTest");
		}
	});
}

void TitleScene::OnExit()
{
	__super::OnExit();
}

void TitleScene::PlayerInput()
{
}

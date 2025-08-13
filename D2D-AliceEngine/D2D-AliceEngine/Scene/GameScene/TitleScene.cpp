#include "TitleScene.h"
#include <Manager/SceneManager.h>
#include "Scripts/TitleUIScript.h"
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/AudioComponent.h>
#include <Scripts/Audio/TitleAudioScript.h>
#include <Core/Input.h>
#include <Component/InputComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Spine2D/SpineScript.h>

void TitleScene::Initialize()
{
	__super::Initialize();

    GamePlayManager& GPM = GamePlayManager::GetInstance();
	GPM.StartGame();


	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<TitleAudioScript>();
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
	SetClickable(true);
	m_cameraController = NewObject<gameObject>(L"Camera");

	m_widget = NewObject<gameObject>(L"Widget");
	//m_UI->AddComponent<TitleUIScript>();
	m_widget->AddComponent<TitleWidgetScript>();

	// 테스트용 컷씬 위젯. 이걸 켜서 확인할 것.
	//m_UI->AddComponent<CutSceneWidgetScript>();

	// 디버그용 씬 전환
	gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
	sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_3)) {
			SceneManager::ChangeScene(Define::Scene_Stage1);
		}
	});
}

void TitleScene::OnExit()
{
	__super::OnExit();
	
	// 타이틀 씬 종료 시 마우스 상태 초기화
	Input::ResetMouseState();

	GamePlayManager::GetInstance().ReleaseTimers();
}

void TitleScene::PlayerInput()
{
}

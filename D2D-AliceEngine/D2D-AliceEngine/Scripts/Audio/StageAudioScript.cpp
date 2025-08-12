#include "StageAudioScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Component/ProgressBarComponent.h>

void StageAudioScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void StageAudioScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void StageAudioScript::Awake()
{
}

void StageAudioScript::OnStart()
{
	m_owner = GetOwner();

	m_Stage1Audio = m_owner->AddComponent<AudioComponent>(L"Stage1");
	m_Stage1Audio->LoadData(L"Bg_music_Stage_1.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Stage2Audio = m_owner->AddComponent<AudioComponent>(L"Stage2");
	m_Stage2Audio->LoadData(L"Bg_music_Stage_2.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Stage3Audio = m_owner->AddComponent<AudioComponent>(L"Stage3");
	m_Stage3Audio->LoadData(L"Bg_music_Stage_Finale.wav", AudioMode::StreamLoop, SoundType::BGM);

	std::wstring SceneName = SceneManager::GetInstance().m_currentScene->GetName();

	if (SceneName == Define::Scene_Stage1)
	{
		m_Stage1Audio->PlayByName(L"Stage1");
	}
	else if (SceneName == Define::Scene_Stage2)
	{
		m_Stage2Audio->PlayByName(L"Stage2");
	}
	else if (SceneName == Define::Scene_Stage3)
	{
		m_Stage3Audio->PlayByName(L"Stage3");
	}
}

void StageAudioScript::OnEnd()
{
	m_Stage1Audio->Stop();
	m_Stage2Audio->Stop();
	m_Stage3Audio->Stop();
}

void StageAudioScript::OnDestroy()
{
}

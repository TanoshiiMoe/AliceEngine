#include "TitleAudioScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>

#include <Component/AudioComponent.h>

void TitleAudioScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void TitleAudioScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TitleAudioScript::Awake()
{
}

void TitleAudioScript::OnStart()
{
	m_owner = GetOwner();
	m_Audio = m_owner->AddComponent<AudioComponent>();
	m_Audio->Load(L"Bg_music_maintitle.wav",AudioMode::StreamLoop);
	m_Audio->Play(0,0.5);
}

void TitleAudioScript::OnEnd()
{
	m_Audio->Stop();
}

void TitleAudioScript::OnDestroy()
{
}

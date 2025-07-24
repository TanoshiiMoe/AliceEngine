#include "Audio.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/AudioComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>

void Audio::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Audio::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

}

void Audio::Awake()
{
}

void Audio::OnStart()
{
	m_sound = GetOwner();
	m_sound->AddComponent<AudioComponent>()->Load(L"audio.mp3",AudioMode::Memory);
	m_sound->GetComponent<AudioComponent>()->Play();
}

void Audio::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void Audio::OnDestroy()
{
}
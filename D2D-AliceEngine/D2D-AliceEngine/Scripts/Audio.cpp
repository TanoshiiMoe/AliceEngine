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
	// ���⿡ Update�� ���� ���� �ۼ�
}

void Audio::Awake()
{
}

void Audio::OnStart()
{
	m_owner = GetOwner();
	m_owner->AddComponent<AudioComponent>()->Load(L"../Resource/Sound/Finder.mp3", AudioMode::StreamLoop);
	m_owner->GetComponent<AudioComponent>()->Play();
	// ���� ����
	//m_owner->GetComponent<AudioComponent>()->SetVolume(0.3f);
}

void Audio::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
	m_owner->GetComponent<AudioComponent>()->Stop();
}

void Audio::OnDestroy()
{
}
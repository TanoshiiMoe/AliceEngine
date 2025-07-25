#include "UI_Script.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>

void UI_Script::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void UI_Script::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void UI_Script::Awake()
{
}

void UI_Script::OnStart()
{
	m_owner = GetOwner();
	//m_owner->AddComponent<UIImage>()->LoadData(L"Sun.png");
	m_owner->AddComponent<UIText>()->SetText(L"asdf");
}

void UI_Script::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void UI_Script::OnDestroy()
{

}

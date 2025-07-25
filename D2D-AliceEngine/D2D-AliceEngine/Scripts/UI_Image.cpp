#include "UI_Image.h"
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

void UI_Image::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void UI_Image::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void UI_Image::Awake()
{
}

void UI_Image::OnStart()
{
	m_owner = GetOwner();
	m_owner->AddComponent<UIImage>()->LoadData(L"Sun.png");
}

void UI_Image::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void UI_Image::OnDestroy()
{

}

#pragma once
#include "../stdafx.h"
#include <Component/InputComponent.h>
#include <Object/gameObject.h>
class MoonInputContext : public InputComponent
{
public:
	void Initialize() override
	{
		__super::Initialize();
	}
	void Update() override
	{
		__super::Update();

		if (GetAsyncKeyState('9') & 0x8000)
		{
			float rot = GetOwner()->m_localTransform.lock()->GetRotation();
			GetOwner()->m_localTransform.lock()->SetRotation(rot + 5.0f);
		}

		if (GetAsyncKeyState('0') & 0x8000)
		{
			float rot = GetOwner()->m_localTransform.lock()->GetRotation();
			GetOwner()->m_localTransform.lock()->SetRotation(rot - 5.0f);
		}
		if (GetAsyncKeyState(VK_O) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x, pos.y - 5.0f);
		}
		if (GetAsyncKeyState(VK_L) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x, pos.y + 5.0f);
		}
		if (GetAsyncKeyState(VK_K) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x - 5.0f, pos.y);
		}
		if (GetAsyncKeyState(VK_OEM_1) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x + 5.0f, pos.y);
		}
	}
	void Release() override
	{
		__super::Release();
	}
};


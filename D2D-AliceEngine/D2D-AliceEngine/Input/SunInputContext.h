#pragma once
#include "../stdafx.h"
#include <Component/InputComponent.h>
#include <Object/gameObject.h>
class SunInputContext : public InputComponent
{
public:
	SunInputContext() {}
	~SunInputContext() {}
public:
	void Initialize() override 
	{ 
		__super::Initialize(); 
	}
	void Update() override
	{
		__super::Update();

		if (GetAsyncKeyState(VK_Z) & 0x8000)
		{
			float rot = GetOwner()->m_localTransform.lock()->GetRotation();
			GetOwner()->m_localTransform.lock()->SetRotation(rot + 5.0f);
		}

		if (GetAsyncKeyState(VK_C) & 0x8000)
		{
			float rot = GetOwner()->m_localTransform.lock()->GetRotation();
			GetOwner()->m_localTransform.lock()->SetRotation(rot - 5.0f);
		}

		if (GetAsyncKeyState(VK_D) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x + 5.0f, pos.y);
		}

		if (GetAsyncKeyState(VK_A) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x - 5.0f, pos.y);
		}

		if (GetAsyncKeyState(VK_W) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x, pos.y - 5.0f);
		}

		if (GetAsyncKeyState(VK_S) & 0x8000)
		{
			auto pos = GetOwner()->m_localTransform.lock()->GetPosition();
			GetOwner()->m_localTransform.lock()->SetPosition(pos.x, pos.y + 5.0f);
		}
	}
	void Release() override 
	{
		__super::Release();
	}
};


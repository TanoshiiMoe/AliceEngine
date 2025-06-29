#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"

class RenderSystem : public SystemBase, public Singleton<RenderSystem>
{
public:
	void Initialize() 
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).lock())
			{
				(*it).lock()->Initialize();
			}
		}
	}
	void UnInitialize()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).lock())
			{
				(*it).lock()->Release();
			}
		}
		UnRegistAll();
	}
};
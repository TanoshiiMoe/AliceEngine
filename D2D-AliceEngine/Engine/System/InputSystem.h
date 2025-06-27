#pragma once
#include "SystemBase.h"

class InputSystem : public SystemBase, public Singleton<InputSystem>
{
public:
	InputSystem() {}
	~InputSystem() {}

	void Update()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).lock())
			{
				(*it).lock()->Update();
			}
		}
	}
};
#pragma once
#include "SystemBase.h"
class TransformSystem : public SystemBase, public Singleton<TransformSystem>
{
public:
	TransformSystem() {}
	~TransformSystem() {}
public: 
	
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


#pragma once
#include "Component.h"
#include "../System/ScriptSystem.h"

class ScriptComponent : public Component
{
public:
	void Initialize() override { ScriptSystem::Get().Regist(this->weak_from_this()); }
	void Release() override { ScriptSystem::Get().UnRegist(this->weak_from_this()); }

	virtual void Update() = 0;
	virtual void OnStart() = 0;
};
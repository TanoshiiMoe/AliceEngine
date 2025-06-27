#pragma once
#include "Component.h"
#include "../System/ScriptSystem.h"

class ScriptComponent : public Component
{
	ScriptComponent() {}
	~ScriptComponent() { ScriptSystem::GetInstance().UnRegist(this->weak_from_this()); }
public:
	void Initialize() override { ScriptSystem::GetInstance().Regist(this->weak_from_this()); }
	void Release() override {  }

	virtual void Update() = 0;
	virtual void OnStart() = 0;
};
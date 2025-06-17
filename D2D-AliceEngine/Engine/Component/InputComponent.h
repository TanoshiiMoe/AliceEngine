#pragma once
#include "Component.h"
#include "System/InputSystem.h"
class InputComponent : public Component
{
public:
	InputComponent() { }
	~InputComponent() { InputSystem::Get().UnRegist(this->weak_from_this()); }
public:
	virtual void Initialize() { InputSystem::Get().Regist(this->weak_from_this()); }
	virtual void Update() {}
	virtual void Release() {}
};
#pragma once
#include "Component.h"
#include "System/InputSystem.h"
class InputComponent : public Component
{
public:
	InputComponent() { InputSystem::Get().Regist(this->weak_from_this()); }
	~InputComponent() { InputSystem::Get().UnRegist(this->weak_from_this()); }
public:
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Release() {}

};
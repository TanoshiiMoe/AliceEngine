#pragma once
#include "SystemBase.h"

class InputSystem : public SystemBase, public Singleton<InputSystem>
{
public:
	InputSystem() {}
	~InputSystem() {}
};
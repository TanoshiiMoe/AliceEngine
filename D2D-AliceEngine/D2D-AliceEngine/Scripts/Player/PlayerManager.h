#pragma once
#include "Component/ScriptComponent.h"
class PlayerManager : public  ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
};


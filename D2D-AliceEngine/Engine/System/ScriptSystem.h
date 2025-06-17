#pragma once
#include "SystemBase.h"
class ScriptSystem : public SystemBase, public Singleton<ScriptSystem>
{
public:
	virtual void Update() = 0;


};


#pragma once
#include "Component.h"

class Scene;
class ScriptComponent : public Component
{
public:
	ScriptComponent();
	~ScriptComponent();
public:
	void Initialize() override;
	void Release() override;

public:
	Scene* GetWorld();

};
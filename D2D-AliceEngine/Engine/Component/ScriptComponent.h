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
	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);
	void Release() override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

public:
	Scene* GetWorld();
};
#pragma once
#include <Component/ScriptComponent.h>
#include <Define/Define.h>

using namespace Define;
class gameObject;
class TitleUIScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;
};
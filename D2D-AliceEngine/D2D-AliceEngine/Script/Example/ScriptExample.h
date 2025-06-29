#pragma once
#include <Component/ScriptComponent.h>

/*
* @briefs :
*	ScriptComponent를 사용하는 예시입니다.
*	이 코드를 복사하여 사용하시면 됩니다.
*/

class gameObject;
class ScriptExample : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void OnStart() override;
	virtual void OnEnd() override;

	void Input();

	gameObject* m_owner;
};


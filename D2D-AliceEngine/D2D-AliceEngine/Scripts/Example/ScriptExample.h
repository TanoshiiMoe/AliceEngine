#pragma once
#include <Component/ScriptComponent.h>

/*
*	@briefs :
*		ScriptComponent�� ����ϴ� �����Դϴ�.
*		�� �ڵ带 �����Ͽ� ����Ͻø� �˴ϴ�.
*
*	@Caution : 
*		C++���� ���÷����� ���⶧���� ����ó�� ����� �Լ��� ������־���մϴ�.
*		Initialize() �κп��� ���ݴϴ�.
*		REGISTER_SCRIPT_METHOD(Awake)
*		REGISTER_SCRIPT_METHOD(OnStart)
*		REGISTER_SCRIPT_METHOD(OnEnd)
*		REGISTER_SCRIPT_METHOD(OnDestroy)
*/

class gameObject;
class ScriptExample : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnCollisionEnter2D(Collision2D* collider) override;
	virtual void OnCollisionStay2D(Collision2D* collider) override;
	virtual void OnCollisionExit2D(Collision2D* collider) override;
	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	gameObject* m_owner;
};


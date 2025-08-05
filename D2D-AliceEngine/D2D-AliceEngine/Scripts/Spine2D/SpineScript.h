#pragma once
#include <Component/ScriptComponent.h>
#include <Spine2D/SpineRenderer.h>

class gameObject;
class SpineScript : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);
	void RenderSpine();

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

	std::unique_ptr<SpineRenderer> spineRenderer;
};


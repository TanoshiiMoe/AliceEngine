#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class BackGroundRender : public ScriptComponent
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

	gameObject* m_owner;

	// background
	gameObject* m_sky;	// 밤하늘 배경(남산타워) // = 1
	gameObject* m_building;	// 건물 배경 // = 2
	gameObject* m_bridge;	// 다리 = 4
	gameObject* m_market;	// 다리 아래 건물 배경 = 5
	gameObject* m_backBarrier; // 방음벽(안) = 6
	gameObject* m_frontBarrier; // 방음벽(밖) = 7
	gameObject* m_guardrail; // 가드레일 = 8

};
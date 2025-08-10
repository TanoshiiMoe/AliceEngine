#pragma once
#include <Component/ScriptComponent.h>

struct LoopingBackGround
{
	gameObject* obj1 = nullptr;
	gameObject* obj2 = nullptr;
    float speed = 1.0f; // 패럴랙스 계수(플레이어 속도에 곱해짐)
	float width = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
};

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
    gameObject* m_sky;	// 밤하늘 배경(남산타워)
	gameObject* m_building;
	//gameObject* m_bridge;
	//gameObject* m_market;
	//gameObject* m_backBarrier;
	//gameObject* m_frontBarrier;
	//gameObject* m_guardrail;

    std::vector<LoopingBackGround> m_loopingLayers;

    // 플레이어 이동 스크립트 (속도 참조)
    class BikeMovementScript* m_playerMove = nullptr;

private:
	void AddLooping(const std::wstring& name,
		const std::wstring& path,
		int layer,
		float y,
		float speed);
};
#pragma once
#include <Component/ScriptComponent.h>

struct LoopingBackGround
{
	gameObject* obj1 = nullptr;
	gameObject* obj2 = nullptr;
	float speed = 100.0f;
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
	gameObject* m_sky;	// ���ϴ� ���(����Ÿ��) // = 1
	gameObject* m_building;	// �ǹ� ��� // = 2
	gameObject* m_bridge;	// �ٸ� = 4
	gameObject* m_market;	// �ٸ� �Ʒ� �ǹ� ��� = 5
	gameObject* m_backBarrier; // ������(��) = 6
	gameObject* m_frontBarrier; // ������(��) = 7
	gameObject* m_guardrail; // ���巹�� = 8

	std::vector<LoopingBackGround> m_loopingLayers;

private:
	void AddLooping(const std::wstring& name,
		const std::wstring& path,
		int layer,
		float y,
		float speed);
};
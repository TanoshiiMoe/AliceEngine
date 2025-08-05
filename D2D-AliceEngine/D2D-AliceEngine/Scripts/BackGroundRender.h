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
	gameObject* m_sky;	// ¹ãÇÏ´Ã ¹è°æ(³²»êÅ¸¿ö)

	//gameObject* m_building;
	//gameObject* m_bridge;
	//gameObject* m_market;
	//gameObject* m_backBarrier;
	//gameObject* m_frontBarrier;
	//gameObject* m_guardrail;

	std::vector<LoopingBackGround> m_loopingLayers;

	gameObject* m_UI_HUD;
	gameObject* m_playerHP;
	gameObject* m_UI_Dashboard;

private:
	void AddLooping(const std::wstring& name,
		const std::wstring& path,
		int layer,
		float y,
		float speed);
};
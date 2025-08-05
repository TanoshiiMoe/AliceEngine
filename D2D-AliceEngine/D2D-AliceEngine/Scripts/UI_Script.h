#pragma once
#include <Component/ScriptComponent.h>

class TextRenderComponent;
class gameObject;
class UI_Script : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;

	gameObject* m_UI_HUD;
	gameObject* m_UI_PlayerHP;
	gameObject* m_UI_TimeText;

	gameObject* m_UI_Pause;

	gameObject* m_UI_Dashboard;
	gameObject* m_UI_BoardText;
	gameObject* m_UI_currSpeed;
	gameObject* m_UI_SpeedText;
	gameObject* m_UI_Battery;

	TextRenderComponent* m_accel;
	TextRenderComponent* m_RealTime;

private:
	float m_realTimer = 0.0f;
	float m_accelTimer = 0.0f;    // 가속준비중 ...
	int m_dotCount = 0;      // 현재 점 개수 (0~3)
};
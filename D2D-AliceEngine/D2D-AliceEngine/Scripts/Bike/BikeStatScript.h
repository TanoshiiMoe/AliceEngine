#pragma once
#include <Component/ScriptComponent.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>
#include <Manager/TimerManager.h>

struct BikeStat {
	float HP = 100.f;
	float MAXHP = 100.0f;
	float BATTERY = 50.f;
	float MAXBATTERY = 50.f;
};

class TextRenderComponent;
DEFINE_STAT_TRAITS_4(BikeStat, HP, MAXHP, BATTERY, MAXBATTERY)

class BikeStatScript : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	void Invoke();
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	void Input();

	//gameObject* m_BikeStat;
	std::vector<TextRenderComponent*> m_BikeNameTexts;
	StatComponent<BikeStat>* m_bikeStat;

	FTimerHandle timer;
};


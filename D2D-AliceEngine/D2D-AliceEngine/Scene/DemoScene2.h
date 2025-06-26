#pragma once
#include <Scene/Scene.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>

struct MyStat {
	float HP = 100.f;
	float MAXHP = 100.0f;
	float MP = 50.f;
	float STR = 10.f;
	float DEX = 20.f;
	float INT = 30.f;
};

DEFINE_STAT_TRAITS_6(MyStat, HP, MAXHP, MP, STR, DEX, INT)

class TextRenderComponent;
class DemoScene2 : public Scene
{
public:
	DemoScene2() {}
	~DemoScene2() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void yuukaInput();
	void aru2Input();

	void aruInput();
	void CameraInput();

	gameObject* m_yuuka;

	gameObject* m_aru;
	gameObject* m_aru2;
	StatComponent<MyStat>* m_aruStat;
	StatComponent<MyStat>* m_aru2Stat;
	StatComponent<>* m_aruDefaultStat;
	std::vector<TextRenderComponent*> m_aruNameTexts;
	std::vector<TextRenderComponent*> m_aru2NameTexts;
	gameObject* m_tree;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};


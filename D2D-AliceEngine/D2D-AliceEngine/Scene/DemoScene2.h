#pragma once
#include <Scene/Scene.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>

struct MyStat {
	float HP = 100.f;
	float MP = 50.f;
	float STR = 10.f;
	float DEX = 20.f;
	float INT = 30.f;
};

DEFINE_STAT_TRAITS_5(MyStat, HP, MP, STR, DEX, INT)

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

	void aruInput();
	void CameraInput();

	gameObject* m_yuuka;

	gameObject* m_aru;
	StatComponent<MyStat>* m_aruStat;
	StatComponent<>* m_aruDefaultStat;
	TextRenderComponent* m_aruNameText;
	TextRenderComponent* m_aruStatText;
	TextRenderComponent* m_aruStatText2;
	gameObject* m_tree;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};


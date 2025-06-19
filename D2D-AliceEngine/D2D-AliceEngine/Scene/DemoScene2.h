#pragma once

#pragma once
#include <Scene/Scene.h>
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
	gameObject* m_tree;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};


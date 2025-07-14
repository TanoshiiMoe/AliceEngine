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

	void aru2Input();

	void aruInput();
	void CameraInput();

	gameObject* m_camera;
	gameObject* m_backgroundImage;

	gameObject* m_aru;
	gameObject* m_aruClone;
	gameObject* m_aru2;
	gameObject* m_alice;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};


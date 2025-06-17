#pragma once
#include <Scene/Scene.h>
class DemoScene : public Scene
{
public:
	DemoScene() {}
	~DemoScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void SunInput();
	void MoonInput();
	void EarthInput();
	void CameraInput();

	gameObject* m_sun;
	gameObject* m_earth;
	gameObject* m_moon;
};


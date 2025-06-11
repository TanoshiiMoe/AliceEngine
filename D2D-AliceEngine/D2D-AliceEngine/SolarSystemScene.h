#pragma once
#include <Scene.h>
#include "SolarSystemObject.h"
class SolarSystemScene : public Scene
{
public:
	SolarSystemScene();
	~SolarSystemScene();

	void Initialize() override;
	void Release() override;
	void Update() override;
	void Render() override;

public:
	std::weak_ptr<SolarSystemObject> m_sun;
	std::weak_ptr<SolarSystemObject> m_earth;
	std::weak_ptr<SolarSystemObject> m_moon;
};


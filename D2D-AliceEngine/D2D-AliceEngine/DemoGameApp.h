#pragma once
#include "stdafx.h"
#include "SolarSystemScene.h"

class DemoGameApp : public Application
{
public:
	DemoGameApp();
	virtual ~DemoGameApp();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Input() override;
	void Uninitialize() override;

public:
	std::shared_ptr<SolarSystemScene> m_solarSystemScene;
};

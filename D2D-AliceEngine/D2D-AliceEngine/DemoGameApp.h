#pragma once
#include "stdafx.h"
#include "Object/gameObject.h"

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
	void Uninitialize() override;

	gameObject* m_sun;
	gameObject* m_earth;
	gameObject* m_moon;
};

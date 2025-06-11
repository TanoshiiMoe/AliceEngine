#pragma once
#include "stdafx.h"
#include "Object/Object.h"

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

	std::weak_ptr<Object> m_sun;
	std::weak_ptr<Object> m_earth;
	std::weak_ptr<Object> m_moon;
};

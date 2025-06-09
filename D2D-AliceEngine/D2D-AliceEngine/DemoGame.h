#pragma once
#include "stdafx.h"
#include "Sun.h"

class DemoGame : public Application
{
public:
	DemoGame();
	virtual ~DemoGame();

public:
	void Initialize() override;
	void Run() override;
	void Uninitialize() override;


	Sun* m_sun; // Sun instance for testing
};

#pragma once
#include "stdafx.h"

class DemoGame : public Application
{
public:
	DemoGame();
	virtual ~DemoGame();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
};

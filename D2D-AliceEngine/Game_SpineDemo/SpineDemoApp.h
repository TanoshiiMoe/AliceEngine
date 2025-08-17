#pragma once
#include <Application.h>

class SpineDemoApp : public Application
{
public:
	SpineDemoApp() = default;
	~SpineDemoApp() override = default;

	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
}; 
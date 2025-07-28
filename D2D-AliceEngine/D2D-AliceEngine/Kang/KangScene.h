#pragma once
#include "Scene/Scene.h"

class KangScene : public Scene {
public:
	KangScene() {};
	~KangScene() {};

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;
};
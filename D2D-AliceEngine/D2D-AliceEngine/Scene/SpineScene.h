#pragma once
#include <Scene/Scene.h>
#include <Spine2D/SpineRenderer.h>

class TextRenderComponent;
class SpineScene : public Scene
{
public:
	SpineScene() {}
	~SpineScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();

	gameObject* spineObject;
};


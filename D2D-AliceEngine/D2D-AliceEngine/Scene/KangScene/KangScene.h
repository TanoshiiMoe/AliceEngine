#pragma once
#include <Scene/Scene.h>

class Canvas;
class TextRenderComponent;
class KangScene : public Scene
{
public:
	KangScene() {}
	~KangScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	gameObject* m_cameraController;

	gameObject* m_player;
	gameObject* m_sound;
	gameObject* m_tile;	// = 3
	Canvas* m_canvas;
	gameObject* m_wall;

	gameObject* m_UI;
	gameObject* m_button;

	gameObject* m_bg;	// �ӽ� ���

	gameObject* m_truck;
};


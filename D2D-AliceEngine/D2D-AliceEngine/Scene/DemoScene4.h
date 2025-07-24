#pragma once
#include <Scene/Scene.h>

// HiroScene 임시 생성
class TextRenderComponent;
class DemoScene4 : public Scene
{
public:
	DemoScene4() {}
	~DemoScene4() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();
	void EnemyInput();

	gameObject* m_player;
	gameObject* m_sound;
	gameObject* m_mouseinput;

	//gameObject* m_backgroundImage;
	//gameObject* m_spriteAnimationTest;
	//gameObject* m_wall;
	//gameObject* m_wall2;
	//gameObject* m_wall3;
	//std::vector<gameObject*> m_enemies;
	//int enemyMax = 20;
	//gameObject* m_widget;
	//gameObject* m_widget2;
	//gameObject* m_widget3;
	//gameObject* m_widget4;
};


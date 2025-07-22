#pragma once
#include <Scene/Scene.h>

/*
*  @briefs : 씬 예시입니다.
*/

class TextRenderComponent;
class ExampleScene : public Scene
{
public:
	ExampleScene() {}
	~ExampleScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();
	void EnemyInput();

	gameObject* m_backgroundImage;
	gameObject* m_spriteAnimationTest;
	gameObject* m_player;
	gameObject* m_wall;
	std::vector<gameObject*> m_enemies;
	int enemyMax = 20;
	gameObject* m_widget;
};


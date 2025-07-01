#pragma once
#include <Scene/Scene.h>

class TextRenderComponent;
class DemoScene3 : public Scene
{
public:
	DemoScene3() {}
	~DemoScene3() {}

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
	//std::vector<std::weak_ptr<gameObject>> m_enemies;
	std::vector<gameObject*> m_enemies;
	int enemyMax = 20;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};


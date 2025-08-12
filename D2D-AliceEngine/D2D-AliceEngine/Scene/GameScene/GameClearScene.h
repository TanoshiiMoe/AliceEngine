#pragma once
#include <Scene/Scene.h>

class GameClearScene : public Scene
{
public:
    GameClearScene() = default;
    ~GameClearScene() = default;

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;

private:
    gameObject* m_textGO{ nullptr };
	gameObject* m_passedTimeText{ nullptr };
	gameObject* m_killEnemyText{ nullptr };

    gameObject* m_sound;
    gameObject* m_widget;
};

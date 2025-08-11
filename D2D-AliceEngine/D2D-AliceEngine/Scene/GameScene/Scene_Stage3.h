#pragma once
#include <Scene/Scene.h>

class gameObject;
class Scene_Stage3 : public Scene
{
public:
    Scene_Stage3() {}
    ~Scene_Stage3() {}

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;

    gameObject* m_cameraController;

    gameObject* m_player;
    gameObject* m_sound;
    gameObject* m_tile;	// = 3
    gameObject* m_wall;

    gameObject* m_UI;
    gameObject* m_button;

    gameObject* m_bg;	// 임시 배경

    gameObject* m_truck;

    gameObject* enemySpawnTriggerBox;

private:
    gameObject* m_textGO{ nullptr };

    // 컷씬 종료 후 전환할 씬명
    std::wstring m_nextSceneName = L"Scene_Stage3";
};



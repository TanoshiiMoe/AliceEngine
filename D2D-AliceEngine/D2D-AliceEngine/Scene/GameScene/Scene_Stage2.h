#pragma once
#include <Scene/Scene.h>

class Scene_Stage2 : public Scene
{
public:
    Scene_Stage2() {}
    ~Scene_Stage2() {}

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;

private:
    gameObject* m_textGO{ nullptr };
};



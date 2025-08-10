#pragma once
#include <Scene/Scene.h>

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

private:
    gameObject* m_textGO{ nullptr };
};



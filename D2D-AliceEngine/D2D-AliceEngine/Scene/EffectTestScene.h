#pragma once
#include <Scene/Scene.h>

class EffectTestScene : public Scene
{
public:
    EffectTestScene() {}
    ~EffectTestScene() {}

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;

private:
    void HandleInput();
    void SpawnVignette(float durationSec, float maxAlpha);
    void SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha);
};



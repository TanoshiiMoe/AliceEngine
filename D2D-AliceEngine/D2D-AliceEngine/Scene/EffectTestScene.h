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

    // a~h 테스트용 스포너
    void SpawnParticleExplosion();      // a
    void SpawnParticleImpact();         // b
    void SpawnParticleClickL();         // c (좌클릭 버스트)
    void SpawnParticleClickR();         // d (우클릭 버스트)
    void ToggleMouseTrail();            // e (토글)
    void SpawnParticleAura();           // f (플레이어 오라 가정, 화면 중앙 근처)
    void SpawnParticleElectric();       // g
    void SpawnParticlePortal();         // h
};



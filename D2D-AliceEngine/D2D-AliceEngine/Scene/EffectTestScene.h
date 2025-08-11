#pragma once
#include <Scene/Scene.h>

class ParticleComponent;
class EffectTestScene : public Scene
{
public:
    EffectTestScene() 
        : colorTransformEffect(nullptr)
        , distortionEffect(nullptr)
        , blendingEffect(nullptr)
        , physicsEffect(nullptr)
        , environmentEffect(nullptr)
        , specialEffect(nullptr)
    {}
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
    void CreateParticleEffects();

    // 6가지 파티클 효과들
    ParticleComponent* colorTransformEffect;    // A키 - 색상 변환
    ParticleComponent* distortionEffect;        // S키 - 왜곡 효과
    ParticleComponent* blendingEffect;          // D키 - 블렌딩 효과
    ParticleComponent* physicsEffect;           // F키 - 물리 시뮬레이션
    ParticleComponent* environmentEffect;       // G키 - 환경 효과
    ParticleComponent* specialEffect;           // H키 - 특수 효과
};



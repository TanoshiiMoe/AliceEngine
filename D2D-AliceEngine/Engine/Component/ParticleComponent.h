#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>
#include <Manager/D2DRenderManager.h>
#include <Math/TMath.h>
#include <vector>
#include <memory>

using namespace Microsoft::WRL;

// 고급 쉐이더 설정
enum class EShaderType
{
	None,
	ColorTransform,    // TestShader1
	Distortion,        // TestShader2
	Blending,          // TestShader3
	Physics,           // TestShader4
	Environment,       // TestShader5
	SpecialEffects     // TestShader6
};

// 파티클 타입 열거형
enum class EParticleType
{
    // 총알 충돌 효과
    BulletHit,          // 총알이 부딪쳤을 때
    BulletSpark,        // 총알 스파크
    
    // 장애물 충돌 효과
    ObstacleHit,        // 장애물에 부딪쳤을 때
    ObstacleBreak,      // 장애물 파괴
    
    // UI/인터랙션 효과
    MouseClick,         // 마우스 클릭
    ButtonHover,        // 버튼 호버
    MenuSelect,         // 메뉴 선택
    
    // 환경 효과
    Explosion,          // 폭발
    Smoke,              // 연기
    Fire,               // 불
    
    // 특수 효과
    Magic,              // 마법
    Healing,            // 힐링
    PowerUp             // 파워업
};

// 개별 파티클 구조체
struct Particle
{
    FVector2 position;      // 월드 위치
    FVector2 velocity;      // 속도
    FVector2 acceleration;  // 가속도
    FVector2 scale;         // 크기
    FVector2 scaleVelocity; // 크기 변화 속도
    
    float rotation;         // 회전
    float rotationSpeed;    // 회전 속도
    
    float life;            // 남은 생명
    float maxLife;         // 최대 생명
    float lifeRatio;       // 생명 비율 (0~1)
    
    FVector4 color;        // 색상 (RGBA)
    FVector4 colorVelocity; // 색상 변화 속도
    
    float opacity;         // 투명도
    float opacityVelocity; // 투명도 변화 속도
    
    bool active;           // 활성화 상태
    
    // 초기화
    void Initialize(const FVector2& pos, const FVector2& vel, float lifeTime);
    
    // 업데이트
    void Update(float deltaTime);
    
    // 쉐이더 파라미터 계산
    void CalculateShaderParams();
};

// 파티클 시스템 설정
struct ParticleSystemConfig
{
    // 기본 설정
    int maxParticles = 100;        // 최대 파티클 수
    float emissionRate = 10.0f;    // 초당 생성 파티클 수
    float duration = 2.0f;         // 시스템 지속 시간 (-1 = 무한)
    
    // 파티클 생명
    float minLife = 0.5f;          // 최소 생명
    float maxLife = 2.0f;          // 최대 생명
    
    // 위치 설정
    FVector2 position = FVector2(0, 0);           // 생성 위치
    FVector2 positionVariance = FVector2(10, 10); // 위치 변화량
    
    // 속도 설정
    FVector2 minVelocity = FVector2(-50, -50);    // 최소 속도
    FVector2 maxVelocity = FVector2(50, 50);      // 최대 속도
    FVector2 acceleration = FVector2(0, 98);      // 가속도 (중력 등)
    
    // 크기 설정
    FVector2 minScale = FVector2(0.5f, 0.5f);    // 최소 크기
    FVector2 maxScale = FVector2(1.5f, 1.5f);    // 최대 크기
    FVector2 scaleVelocity = FVector2(-0.5f, -0.5f); // 크기 변화 속도
    
    // 회전 설정
    float minRotationSpeed = -180.0f;              // 최소 회전 속도
    float maxRotationSpeed = 180.0f;               // 최대 회전 속도
    
    // 색상 설정
    FVector4 startColor = FVector4(1, 1, 1, 1);  // 시작 색상
    FVector4 endColor = FVector4(1, 1, 1, 0);    // 끝 색상
    FVector4 colorVariance = FVector4(0.2f, 0.2f, 0.2f, 0.1f); // 색상 변화량
    
    // 투명도 설정
    float startOpacity = 1.0f;                    // 시작 투명도
    float endOpacity = 0.0f;                      // 끝 투명도
    
    // 쉐이더 효과
    bool useShader = false;                       // 쉐이더 사용 여부
    float shaderTime = 0.0f;                     // 쉐이더 시간
    float shaderIntensity = 1.0f;                // 쉐이더 강도
	
	EShaderType shaderType = EShaderType::None;
	
	// 물리 시뮬레이션 설정
	bool usePhysics = false;                      // 물리 시뮬레이션 사용 여부
	float gravity = 98.0f;                        // 중력
	float viscosity = 0.1f;                       // 점성
	float surfaceTension = 0.5f;                  // 표면 장력
	float pressure = 1.0f;                        // 압력
	
	// 환경 효과 설정
	bool useEnvironment = false;                   // 환경 효과 사용 여부
	float weather = 0.0f;                         // 날씨 강도 (0-1)
	float lighting = 1.0f;                        // 조명 강도
	float reflection = 0.3f;                      // 반사 강도
	float refraction = 0.2f;                      // 굴절 강도
	
	// 특수 효과 설정
	bool useSpecialEffects = false;                // 특수 효과 사용 여부
	float magicIntensity = 1.0f;                  // 마법 강도
	float explosionPower = 1.0f;                  // 폭발 강도
	float electricCharge = 1.0f;                  // 전기 전하
	float portalStrength = 1.0f;                  // 포털 강도
};

class ParticleComponent : public RenderComponent
{
public:
    ParticleComponent();
    ~ParticleComponent();

    void Initialize() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;
    void Release() override;

    // 파티클 시스템 설정
    void SetParticleType(EParticleType type);
    void SetConfig(const ParticleSystemConfig& config);
    void SetTexture(const std::wstring& texturePath);
    
    // 파티클 시스템 제어
    void Play();
    void Stop();
    void Pause();
    void Reset();
    
    // 파티클 생성
    void EmitParticle();
    void EmitBurst(int count);
    
    // 쉐이더 설정
    void SetShaderEffect(bool useShader, float intensity = 1.0f);
    void SetShaderType(EShaderType type);
    
    // 물리 시뮬레이션 설정
    void SetPhysicsSimulation(bool usePhysics, float gravity = 98.0f, float viscosity = 0.1f);
    void SetSurfaceTension(float tension);
    void SetPressure(float pressure);
    
    // 환경 효과 설정
    void SetEnvironmentEffects(bool useEnvironment, float weather = 0.0f, float lighting = 1.0f);
    void SetReflectionRefraction(float reflection, float refraction);
    
    // 특수 효과 설정
    void SetSpecialEffects(bool useSpecial, float magic = 1.0f, float explosion = 1.0f, float electric = 1.0f, float portal = 1.0f);
    
    // 위치 설정
    void SetEmissionPosition(const FVector2& position);
    void SetEmissionArea(const FVector2& size);

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;
	FVector2 GetBitmapSize();

private:
    // 파티클 관리
    void UpdateParticles(float deltaTime);
    void RenderParticles();
    void CleanupDeadParticles();
    
    // 파티클 풀 관리
    Particle* GetAvailableParticle();
    void ReturnParticle(Particle* particle);
    
    // 쉐이더 효과
    void SetupShaderEffects();
    void UpdateShaderParameters();
    
    // 타입별 설정
    void ApplyTypeConfig(EParticleType type);
    
    // SpriteBatch 렌더링
    void AddParticleToSpriteBatch(const Particle& particle);

private:
    // 파티클 시스템
    std::vector<std::unique_ptr<Particle>> m_particles;
    std::vector<Particle*> m_activeParticles;
    std::vector<Particle*> m_particlePool;
    
    // 설정
    EParticleType m_particleType;
    ParticleSystemConfig m_config;
    
    // 상태
    bool m_isPlaying;
    bool m_isPaused;
    float m_emissionTimer;
    float m_systemTimer;
    int m_particlesEmitted;
    
    // 텍스처
    std::wstring m_texturePath;
    std::shared_ptr<ID2D1Bitmap1> m_texture;
    
    // 쉐이더
    ComPtr<ID2D1Effect> m_colorMatrixEffect;
    ComPtr<ID2D1Effect> m_blendEffect;
    ComPtr<ID2D1Effect> m_gaussianBlurEffect;
    
    // SpriteBatch
    ID2D1SpriteBatch* m_spriteBatch;
    
    // 렌더링 최적화
    bool m_needsRebuild;
    int m_lastActiveCount;
};

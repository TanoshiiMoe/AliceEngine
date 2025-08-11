#include "pch.h"
#include "ParticleComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Helpers/CoordHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Core/StatTraits.h>

// Particle ����ü ����
void Particle::Initialize(const FVector2& pos, const FVector2& vel, float lifeTime)
{
    position = pos;
    velocity = vel;
    acceleration = FVector2(0, 0);
    scale = FVector2(1, 1);
    scaleVelocity = FVector2(0, 0);
    
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    
    life = lifeTime;
    maxLife = lifeTime;
    lifeRatio = 1.0f;
    
    color = FVector4(1, 1, 1, 1);
    colorVelocity = FVector4(0, 0, 0, 0);
    
    opacity = 1.0f;
    opacityVelocity = 0.0f;
    
    active = true;
}

void Particle::Update(float deltaTime)
{
    if (!active) return;
    
    // ���� ����
    life -= deltaTime;
    if (life <= 0.0f)
    {
        active = false;
        return;
    }
    
    // ���� ���� ���
    lifeRatio = life / maxLife;
    
    // ��ġ ������Ʈ
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    // ũ�� ������Ʈ
    scale += scaleVelocity * deltaTime;
    
    // ȸ�� ������Ʈ
    rotation += rotationSpeed * deltaTime;
    
    // ���� ������Ʈ
    color += colorVelocity * deltaTime;
    
    // ���� ������Ʈ
    opacity += opacityVelocity * deltaTime;
    
    // ���̴� �Ķ���� ���
    CalculateShaderParams();
}

void Particle::CalculateShaderParams()
{
    // ���� ���� ����� ���� ����
    // �� �Լ��� ���̴����� ����� �Ķ���͵��� �̸� ���
}

// ParticleComponent ����
ParticleComponent::ParticleComponent()
    : m_particleType(EParticleType::BulletHit)
    , m_isPlaying(false)
    , m_isPaused(false)
    , m_emissionTimer(0.0f)
    , m_systemTimer(0.0f)
    , m_particlesEmitted(0)
    , m_spriteBatch(nullptr)
    , m_needsRebuild(false)
    , m_lastActiveCount(0)
{
    // �⺻ ���� ����
    ApplyTypeConfig(m_particleType);
}

ParticleComponent::~ParticleComponent()
{
    Release();
}

void ParticleComponent::Initialize()
{
    __super::Initialize();
    
    // SpriteBatch ����
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (context)
    {
        HRESULT hr = context->CreateSpriteBatch(&m_spriteBatch);
        if (SUCCEEDED(hr))
        {
            m_spriteBatch->AddRef(); // ���� ī��Ʈ ����
        }
    }
    
    // ��ƼŬ Ǯ �ʱ�ȭ
    m_particles.resize(m_config.maxParticles);
    for (auto& particle : m_particles)
    {
        particle = std::make_unique<Particle>();
        m_particlePool.push_back(particle.get());
    }
    
    // ���̴� ȿ�� ����
    SetupShaderEffects();
}

void ParticleComponent::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    
    if (!m_isPlaying || m_isPaused) return;
    
    // �ý��� Ÿ�̸� ������Ʈ
    if (m_config.duration > 0)
    {
        m_systemTimer += deltaSeconds;
        if (m_systemTimer >= m_config.duration)
        {
            Stop();
            return;
        }
    }
    
    // ��ƼŬ ����
    m_emissionTimer += deltaSeconds;
    float emissionInterval = 1.0f / m_config.emissionRate;
    
    while (m_emissionTimer >= emissionInterval)
    {
        EmitParticle();
        m_emissionTimer -= emissionInterval;
    }
    
    // ��ƼŬ ������Ʈ
    UpdateParticles(deltaSeconds);
    
    // ���� ��ƼŬ ����
    CleanupDeadParticles();
    
    // ������ ����ȭ üũ
    if (m_lastActiveCount != m_activeParticles.size())
    {
        m_needsRebuild = true;
        m_lastActiveCount = m_activeParticles.size();
    }
}

void ParticleComponent::Render()
{
    if (!m_isPlaying || m_activeParticles.empty()) return;
    
    __super::Render();
    
    // SpriteBatch�� ����� ��ġ ������
    if (m_spriteBatch && m_texture)
    {
        RenderParticles();
    }
}

void ParticleComponent::Release()
{
    // Ÿ�̸� ����
    m_isPlaying = false;
    
    // ��ƼŬ ����
    m_particles.clear();
    m_activeParticles.clear();
    m_particlePool.clear();
    
    // SpriteBatch ����
    if (m_spriteBatch)
    {
        m_spriteBatch->Release();
        m_spriteBatch = nullptr;
    }
    
    // ���̴� ����
    m_colorMatrixEffect.Reset();
    m_blendEffect.Reset();
    m_gaussianBlurEffect.Reset();
    
    __super::Release();
}

void ParticleComponent::SetParticleType(EParticleType type)
{
    m_particleType = type;
    ApplyTypeConfig(type);
}

void ParticleComponent::SetConfig(const ParticleSystemConfig& config)
{
    m_config = config;
    
    // ��ƼŬ Ǯ ũ�� ����
    if (m_config.maxParticles != m_particles.size())
    {
        m_particles.resize(m_config.maxParticles);
        m_particlePool.clear();
        m_activeParticles.clear();
        
        for (auto& particle : m_particles)
        {
            particle = std::make_unique<Particle>();
            m_particlePool.push_back(particle.get());
        }
    }
}

void ParticleComponent::SetTexture(const std::wstring& texturePath)
{
    m_texturePath = texturePath;
    m_texture = PackageResourceManager::GetInstance().CreateBitmapFromFile(
        (Define::BASE_RESOURCE_PATH + texturePath).c_str());
}

void ParticleComponent::Play()
{
    m_isPlaying = true;
    m_isPaused = false;
    m_systemTimer = 0.0f;
    m_emissionTimer = 0.0f;
    m_particlesEmitted = 0;
}

void ParticleComponent::Stop()
{
    m_isPlaying = false;
    m_isPaused = false;
    
    // ��� ��ƼŬ ��Ȱ��ȭ
    for (auto* particle : m_activeParticles)
    {
        particle->active = false;
    }
    m_activeParticles.clear();
    
    // ��ƼŬ Ǯ�� ��ȯ
    for (auto& particle : m_particles)
    {
        if (particle->active)
        {
            particle->active = false;
            m_particlePool.push_back(particle.get());
        }
    }
}

void ParticleComponent::Pause()
{
    m_isPaused = !m_isPaused;
}

void ParticleComponent::Reset()
{
    Stop();
    m_systemTimer = 0.0f;
    m_emissionTimer = 0.0f;
    m_particlesEmitted = 0;
}

void ParticleComponent::EmitParticle()
{
    if (m_particlePool.empty()) return;
    
    Particle* particle = GetAvailableParticle();
    if (!particle) return;
    
    // ��ġ ��� (�⺻ ��ġ + ��ȭ��)
    FVector2 pos = m_config.position;
    pos.x += FRandom::GetRandomInRange(-m_config.positionVariance.x, m_config.positionVariance.x);
    pos.y += FRandom::GetRandomInRange(-m_config.positionVariance.y, m_config.positionVariance.y);
    
    // �ӵ� ���
    FVector2 vel;
    vel.x = FRandom::GetRandomInRange(m_config.minVelocity.x, m_config.maxVelocity.x);
    vel.y = FRandom::GetRandomInRange(m_config.minVelocity.y, m_config.maxVelocity.y);
    
    // ���� ���
    float life = FRandom::GetRandomInRange(m_config.minLife, m_config.maxLife);
    
    // ��ƼŬ �ʱ�ȭ
    particle->Initialize(pos, vel, life);
    
    // ���� ����
    particle->acceleration = m_config.acceleration;
    particle->scale = FVector2(
        FRandom::GetRandomInRange(m_config.minScale.x, m_config.maxScale.x),
        FRandom::GetRandomInRange(m_config.minScale.y, m_config.maxScale.y)
    );
    particle->scaleVelocity = m_config.scaleVelocity;
    particle->rotationSpeed = FRandom::GetRandomInRange(m_config.minRotationSpeed, m_config.maxRotationSpeed);
    
    // ���� ����
    particle->color = m_config.startColor;
    particle->colorVelocity = (m_config.endColor - m_config.startColor) / life;
    
    // ���� ����
    particle->opacity = m_config.startOpacity;
    particle->opacityVelocity = (m_config.endOpacity - m_config.startOpacity) / life;
    
    // Ȱ�� ��ƼŬ ��Ͽ� �߰�
    m_activeParticles.push_back(particle);
    m_particlesEmitted++;
}

void ParticleComponent::EmitBurst(int count)
{
    for (int i = 0; i < count; ++i)
    {
        EmitParticle();
    }
}

void ParticleComponent::SetShaderEffect(bool useShader, float intensity)
{
	m_config.useShader = useShader;
	m_config.shaderIntensity = intensity;
	
	if (useShader)
	{
		SetupShaderEffects();
	}
}

void ParticleComponent::SetShaderType(EShaderType type)
{
	m_config.shaderType = type;
	
	// ���̴� Ÿ�Կ� ���� ���� ����
	switch (type)
	{
	case EShaderType::ColorTransform:
		// ���� ��ȯ ���̴� ����
		break;
	case EShaderType::Distortion:
		// �ְ� ���̴� ����
		break;
	case EShaderType::Blending:
		// ���� ���̴� ����
		break;
	case EShaderType::Physics:
		// ���� ���̴� ����
		break;
	case EShaderType::Environment:
		// ȯ�� ���̴� ����
		break;
	case EShaderType::SpecialEffects:
		// Ư�� ȿ�� ���̴� ����
		break;
	default:
		break;
	}
}

void ParticleComponent::SetPhysicsSimulation(bool usePhysics, float gravity, float viscosity)
{
	m_config.usePhysics = usePhysics;
	m_config.gravity = gravity;
	m_config.viscosity = viscosity;
}

void ParticleComponent::SetSurfaceTension(float tension)
{
	m_config.surfaceTension = tension;
}

void ParticleComponent::SetPressure(float pressure)
{
	m_config.pressure = pressure;
}

void ParticleComponent::SetEnvironmentEffects(bool useEnvironment, float weather, float lighting)
{
	m_config.useEnvironment = useEnvironment;
	m_config.weather = weather;
	m_config.lighting = lighting;
}

void ParticleComponent::SetReflectionRefraction(float reflection, float refraction)
{
	m_config.reflection = reflection;
	m_config.refraction = refraction;
}

void ParticleComponent::SetSpecialEffects(bool useSpecial, float magic, float explosion, float electric, float portal)
{
	m_config.useSpecialEffects = useSpecial;
	m_config.magicIntensity = magic;
	m_config.explosionPower = explosion;
	m_config.electricCharge = electric;
	m_config.portalStrength = portal;
}

// GetBitmapSize �Լ��� ����
float ParticleComponent::GetBitmapSizeX()
{
	if (m_texture)
	{
		D2D1_SIZE_F size = m_texture->GetSize();
		return size.width;
	}
	return 32.0f; // �⺻ ũ��
}

float ParticleComponent::GetBitmapSizeY()
{
	if (m_texture)
	{
		D2D1_SIZE_F size = m_texture->GetSize();
		return size.height;
	}
	return 32.0f; // �⺻ ũ��
}

FVector2 ParticleComponent::GetBitmapSize()
{
	return FVector2(GetBitmapSizeX(), GetBitmapSizeY());
}

void ParticleComponent::SetEmissionPosition(const FVector2& position)
{
    m_config.position = position;
}

void ParticleComponent::SetEmissionArea(const FVector2& size)
{
    m_config.positionVariance = size * 0.5f;
}

// Private �޼����
void ParticleComponent::UpdateParticles(float deltaTime)
{
    for (auto* particle : m_activeParticles)
    {
        particle->Update(deltaTime);
    }
}

void ParticleComponent::RenderParticles()
{
    if (!m_spriteBatch || !m_texture) return;
    
    // SpriteBatch Ŭ����
    m_spriteBatch->Clear();
    
    // Ȱ�� ��ƼŬ���� SpriteBatch�� �߰�
    for (auto* particle : m_activeParticles)
    {
        if (particle->active)
        {
            AddParticleToSpriteBatch(*particle);
        }
    }
    
    // ��ġ ������
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (context && m_spriteBatch->GetSpriteCount() > 0)
    {
        // ���̴� ȿ�� ����
        		if (m_config.useShader && m_colorMatrixEffect)
		{
			context->DrawImage(static_cast<ID2D1Image*>(m_colorMatrixEffect.Get()));
		}
		else
		{
			context->DrawSpriteBatch(m_spriteBatch, 0, m_spriteBatch->GetSpriteCount(), m_texture.get());
		}
    }
}

void ParticleComponent::CleanupDeadParticles()
{
    // ���� ��ƼŬ���� Ǯ�� ��ȯ
    for (auto it = m_activeParticles.begin(); it != m_activeParticles.end();)
    {
        if (!(*it)->active)
        {
            m_particlePool.push_back(*it);
            it = m_activeParticles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Particle* ParticleComponent::GetAvailableParticle()
{
    if (m_particlePool.empty()) return nullptr;
    
    Particle* particle = m_particlePool.back();
    m_particlePool.pop_back();
    return particle;
}

void ParticleComponent::ReturnParticle(Particle* particle)
{
    if (particle)
    {
        particle->active = false;
        m_particlePool.push_back(particle);
    }
}

void ParticleComponent::SetupShaderEffects()
{
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (!context) return;
    
    // ���� ��Ʈ���� ȿ�� (���� ����)
    if (SUCCEEDED(context->CreateEffect(CLSID_D2D1ColorMatrix, &m_colorMatrixEffect)))
    {
        // �⺻ ���� ��Ʈ���� ����
        D2D1_MATRIX_5X4_F colorMatrix = D2D1::Matrix5x4F(
            1.0f, 0.0f, 0.0f, 0.0f,  // Red
            0.0f, 1.0f, 0.0f, 0.0f,  // Green
            0.0f, 0.0f, 1.0f, 0.0f,  // Blue
            0.0f, 0.0f, 0.0f, 1.0f,  // Alpha
            0.0f, 0.0f, 0.0f, 0.0f   // Offset
        );
        m_colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
    }
    
    // ���� ȿ��
    if (SUCCEEDED(context->CreateEffect(CLSID_D2D1Blend, &m_blendEffect)))
    {
        m_blendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_MULTIPLY);
    }
    
    // ����þ� �� ȿ��
    if (SUCCEEDED(context->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlurEffect)))
    {
        m_gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 2.0f);
    }
}

void ParticleComponent::UpdateShaderParameters()
{
    if (!m_config.useShader) return;
    
    // �ð��� ���� ���̴� �Ķ���� ������Ʈ
    m_config.shaderTime += 0.016f; // 60fps ����
    
    // ���� ��Ʈ���� ȿ�� ������Ʈ
    if (m_colorMatrixEffect)
    {
        // �ð��� ���� ���� ��ȭ
        float time = m_config.shaderTime;
        float intensity = m_config.shaderIntensity;
        
        D2D1_MATRIX_5X4_F colorMatrix = D2D1::Matrix5x4F(
            1.0f + std::sin(time) * 0.1f * intensity, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f + std::cos(time) * 0.1f * intensity, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f + std::sin(time * 0.5f) * 0.1f * intensity, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        );
        
        m_colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
    }
}

void ParticleComponent::ApplyTypeConfig(EParticleType type)
{
    switch (type)
    {
    case EParticleType::BulletHit:
        m_config.maxParticles = 20;
        m_config.emissionRate = 30.0f;
        m_config.duration = 0.5f;
        m_config.minLife = 0.2f;
        m_config.maxLife = 0.6f;
        m_config.minVelocity = FVector2(-100, -100);
        m_config.maxVelocity = FVector2(100, 100);
        m_config.acceleration = FVector2(0, 200);
        m_config.minScale = FVector2(0.3f, 0.3f);
        m_config.maxScale = FVector2(0.8f, 0.8f);
        m_config.startColor = FVector4(1, 0.8f, 0.2f, 1);
        m_config.endColor = FVector4(1, 0.4f, 0, 0);
        break;
        
    case EParticleType::BulletSpark:
        m_config.maxParticles = 15;
        m_config.emissionRate = 25.0f;
        m_config.duration = 0.3f;
        m_config.minLife = 0.1f;
        m_config.maxLife = 0.4f;
        m_config.minVelocity = FVector2(-150, -150);
        m_config.maxVelocity = FVector2(150, 150);
        m_config.acceleration = FVector2(0, 300);
        m_config.minScale = FVector2(0.2f, 0.2f);
        m_config.maxScale = FVector2(0.6f, 0.6f);
        m_config.startColor = FVector4(1, 1, 0.5f, 1);
        m_config.endColor = FVector4(1, 0.5f, 0, 0);
        break;
        
    case EParticleType::ObstacleHit:
        m_config.maxParticles = 25;
        m_config.emissionRate = 40.0f;
        m_config.duration = 0.8f;
        m_config.minLife = 0.3f;
        m_config.maxLife = 0.8f;
        m_config.minVelocity = FVector2(-80, -80);
        m_config.maxVelocity = FVector2(80, 80);
        m_config.acceleration = FVector2(0, 150);
        m_config.minScale = FVector2(0.4f, 0.4f);
        m_config.maxScale = FVector2(1.0f, 1.0f);
        m_config.startColor = FVector4(0.8f, 0.8f, 0.8f, 1);
        m_config.endColor = FVector4(0.4f, 0.4f, 0.4f, 0);
        break;
        
    case EParticleType::MouseClick:
        m_config.maxParticles = 12;
        m_config.emissionRate = 20.0f;
        m_config.duration = 0.4f;
        m_config.minLife = 0.2f;
        m_config.maxLife = 0.5f;
        m_config.minVelocity = FVector2(-60, -60);
        m_config.maxVelocity = FVector2(60, 60);
        m_config.acceleration = FVector2(0, 100);
        m_config.minScale = FVector2(0.5f, 0.5f);
        m_config.maxScale = FVector2(1.2f, 1.2f);
        m_config.startColor = FVector4(0.2f, 0.6f, 1, 1);
        m_config.endColor = FVector4(0.1f, 0.3f, 0.8f, 0);
        break;
        
    case EParticleType::Explosion:
        m_config.maxParticles = 50;
        m_config.emissionRate = 60.0f;
        m_config.duration = 1.0f;
        m_config.minLife = 0.5f;
        m_config.maxLife = 1.2f;
        m_config.minVelocity = FVector2(-200, -200);
        m_config.maxVelocity = FVector2(200, 200);
        m_config.acceleration = FVector2(0, 50);
        m_config.minScale = FVector2(0.8f, 0.8f);
        m_config.maxScale = FVector2(2.0f, 2.0f);
        m_config.startColor = FVector4(1, 0.5f, 0, 1);
        m_config.endColor = FVector4(1, 0, 0, 0);
        m_config.useShader = true;
        break;
        
    default:
        // �⺻ ���� ����
        break;
    }
}

void ParticleComponent::AddParticleToSpriteBatch(const Particle& particle)
{
    if (!m_spriteBatch || !m_texture) return;
    
    // ��ƼŬ�� ���� ��ġ�� ȭ�� ��ǥ�� ��ȯ
    FVector2 screenPos = CoordHelper::WorldToScreen(particle.position);
    
    // ��ƼŬ ũ�� ���
    FVector2 size = particle.scale * GetBitmapSize();
    
    // ������ �簢�� (ȭ�� ��ǥ)
    D2D1_RECT_F destRect = D2D1::RectF(
        screenPos.x - size.x * 0.5f,
        screenPos.y - size.y * 0.5f,
        screenPos.x + size.x * 0.5f,
        screenPos.y + size.y * 0.5f
    );
    
    // �ҽ� �簢�� (�ؽ�ó ��ǥ)
    D2D1_RECT_U srcRect = D2D1::RectU(0, 0, 
        static_cast<UINT32>(GetBitmapSizeX()), 
        static_cast<UINT32>(GetBitmapSizeY()));
    
    // SpriteBatch�� �߰�
    m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, nullptr);
}

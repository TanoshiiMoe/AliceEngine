#include "ParticleHelper.h"
#include <Manager/SceneManager.h>
#include <Scene/Scene.h>
#include <Component/Effect/ParticleComponent.h>
#include <Core/Input.h>
#include <Object/Camera.h>

// =============== 파티클 테스트 스폰 구현 ===============
void ParticleHelper::SpawnParticleExplosion(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldExplosion");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldExplosion");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::WorldSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitExplosion(FVector2(0, 0));
	}
}

void ParticleHelper::SpawnParticleImpact(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldImpact");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldImpact");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::WorldSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitImpact(FVector2(0,0));
	}
}

void ParticleHelper::SpawnParticleClickL(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenClickL");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenClickL");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::ScreenSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitClickBurst(FVector2(0, 0), false);
	}
}

void ParticleHelper::SpawnParticleClickR(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenClickR");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenClickR");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::ScreenSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitClickBurst(FVector2(0, 0), true);
	}
}

void ParticleHelper::ToggleMouseTrail()
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenTrail");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenTrail");
		}
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::ScreenSpace);
		static bool on = false;
		on = !on;
		pc->ToggleMouseTrail(on);
	}
}

void ParticleHelper::SpawnParticleAura(const FVector2& pos, float radius, int count, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldAura");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldAura");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::WorldSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitAura(pos, radius > 0 ? radius : 100.0f, count);
	}
}

void ParticleHelper::SpawnParticleElectric(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldElectric");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldElectric");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::WorldSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitElectric(FVector2(0, 0));
	}
}

void ParticleHelper::SpawnParticlePortal(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldPortal");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldPortal");
		}
		go->SetPosition(pos);
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->SetDrawType(Define::EDrawType::WorldSpace);
		if (!texturePath.empty()) pc->LoadData(texturePath);
		pc->SetAdditiveBlend(true);
		pc->EmitPortalSwirl(FVector2(0, 0));
	}
}

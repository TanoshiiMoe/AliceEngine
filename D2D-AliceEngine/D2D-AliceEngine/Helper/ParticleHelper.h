#pragma once
#include <Math/TMath.h>
class ParticleHelper
{
public:
	static void SpawnParticleExplosion(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleImpact(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleClickL(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleClickR(const FVector2& pos, const std::wstring& texturePath = L"");
	static void ToggleMouseTrail();
	static void SpawnParticleAura(const FVector2& pos, float radius, int count, const std::wstring& texturePath = L"");
	static void SpawnParticleElectric(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticlePortal(const FVector2& pos, const std::wstring& texturePath = L"");
};


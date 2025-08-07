#include "BulletManager.h"
#include <Scripts/Weapon/Bullet.h>
#include <Manager/SceneManager.h>

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::FireBullet(const FVector2& start, const FVector2& target)
{
	Scene* curScene =SceneManager::GetInstance().GetWorld();
	auto bulletObj = curScene->NewObject<gameObject>(L"bullet");
	bulletObj->SetPosition(start);
	auto bullet = bulletObj->AddComponent<Bullet>();

	bullet->P0 = start;
	bullet->P2 = target;

	//컨트롤 포인트 계산: 시작점과 끝점 중간에서 위로 올림
	FVector2 mid = (start + target) * 0.5f;
	FVector2 offset(0, -100.0f); // 위로 꺾이게
	bullet->P1 = mid + offset;

	bullets.push_back(bullet);
}

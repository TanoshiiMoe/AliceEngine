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

	//��Ʈ�� ����Ʈ ���: �������� ���� �߰����� ���� �ø�
	FVector2 mid = (start + target) * 0.5f;
	FVector2 offset(0, -100.0f); // ���� ���̰�
	bullet->P1 = mid + offset;

	bullets.push_back(bullet);
}

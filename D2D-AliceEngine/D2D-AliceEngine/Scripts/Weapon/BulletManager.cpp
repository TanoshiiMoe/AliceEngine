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

	FVector2 dir = target - start;
	float distance = dir.Length();
	dir.Normalize();
	FVector2 perp(-dir.y, dir.x); // ���� ����

	//��Ʈ�� ����Ʈ ���: �������� ���� �߰����� ���� �ø�
	FVector2 mid = (start + target) * 0.5f;

	// �Ÿ� ������� �ִ� ���� ���� ��� (��: �Ÿ��� 10%)
	float curveRatio = 0.001f; // �̰� ���߸� �� ��
	float curveAmount = distance * curveRatio;

	// �¿� ���� �ֱ�
	float sign = (rand() % 2 == 0) ? 1.0f : -1.0f;
	FVector2 offset = perp * curveAmount * sign;

	bullet->P0 = start;
	bullet->P1 = mid + offset;
	bullet->P2 = target;

	bullets.push_back(bullet);
}

#include "BulletManager.h"
#include <Scripts/Weapon/Bullet.h>
#include <Manager/SceneManager.h>
#include <Math/TMath.h>

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::FireBullet(const FVector2& start, const FVector2& target, const FVector2& initVelocity)
{
	Scene* curScene =SceneManager::GetInstance().GetWorld();
	auto bulletObj = curScene->NewObject<gameObject>(L"bullet");
	bulletObj->SetPosition(start);
	bulletObj->SetScale(FVector2(0.2, 0.2));
	auto bullet = bulletObj->AddComponent<Bullet>();
	bullet->inheritedVelocity = initVelocity;

	FVector2 dir = target - start;
	float distance = dir.Length();
	FVector2 dirNormal = dir.Normalize();
	FVector2 perp(-dirNormal.y, dirNormal.x); // ���� ����

	float randMultiply = FRandom::GetRandomInRange(0.1, 0.9);
	//��Ʈ�� ����Ʈ ���: �������� ���� �߰����� ���� �ø�
	FVector2 mid = (start + target) * randMultiply;

	// �Ÿ� ������� �ִ� ���� ���� ��� (��: �Ÿ��� 10%)
	float randCurveMultiply = FRandom::GetRandomInRange(0.05f, 0.07f);
	float curveRatio = randCurveMultiply; // �̰� ���߸� �� ��
	float curveAmount = distance * curveRatio;

	// �¿� ���� �ֱ�
	float sign = (rand() % 2 == 0) ? 1.0f : -1.0f;
	FVector2 offset = perp * curveAmount * sign;

	bullet->P0 = start;
	bullet->P1 = mid + offset;
	bullet->P2 = target;

	bullet->startPos = start;
	bullet->currentPos = start;
	bullet->moveDir = dirNormal;

	bullets.push_back(bullet);
}

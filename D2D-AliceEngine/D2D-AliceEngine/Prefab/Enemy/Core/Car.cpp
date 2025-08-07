#include "Car.h"
#include <Component/Animator.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"

void Car::Initialize()
{
	__super::Initialize();

	//�ִϸ����� ������
	//owner->AddComponent<Animator>();
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// ��ũ��Ʈ
	owner->AddComponent<EnemyManager>();
}

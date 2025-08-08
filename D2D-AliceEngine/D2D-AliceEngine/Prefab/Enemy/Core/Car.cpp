#include "Car.h"
#include <Component/Animator.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"
#include "System/ScriptSystem.h"

void Car::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	//�ִϸ����� ������
	//owner->AddComponent<Animator>();
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// ��ũ��Ʈ
	owner->AddComponent<EnemyManager>();
}

void Car::OnStart()
{
	// SkewTransform���� ��ȯ�ϱ�
	if (auto st = owner->GetComponent<SkewTransform>())
		st->ToSkewPos();
}

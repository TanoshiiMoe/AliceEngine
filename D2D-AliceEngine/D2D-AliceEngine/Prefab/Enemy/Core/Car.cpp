#include "Car.h"
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"
#include "System/ScriptSystem.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimatorInstance.h"

void Car::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	// �ִϸ����� ������
	owner->AddComponent<AnimatorInstance>();
	// ������
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// ��ũ��Ʈ
	owner->AddComponent<EnemyManager>();
}

void Car::OnStart()
{
	// SkewTransform���� ��ȯ�ϱ�
	SkewTransform* st = owner->GetComponent<SkewTransform>();
	st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");
	st->ToSkewPos();
}

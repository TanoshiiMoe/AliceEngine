#include "NormalTruck.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"

void NormalTruck::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void NormalTruck::OnStart()
{
	__super::OnStart();
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Porter/porter.png");
	owner->transform()->SetScale(0.6f, 0.6f);
}

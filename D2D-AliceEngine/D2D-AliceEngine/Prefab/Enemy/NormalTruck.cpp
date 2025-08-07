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
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Porter/porter.png");
}

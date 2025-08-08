#include "JumpTruck.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"

void JumpTruck::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void JumpTruck::OnStart()
{
	__super::OnStart();

	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Jumporter/jumporter.png");
	owner->transform()->SetScale(0.35f, 0.35f);
}

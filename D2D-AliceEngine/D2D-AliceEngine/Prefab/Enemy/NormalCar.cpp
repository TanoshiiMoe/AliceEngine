#include "NormalCar.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"

void NormalCar::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void NormalCar::OnStart()
{
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Tico/tico.png");
}

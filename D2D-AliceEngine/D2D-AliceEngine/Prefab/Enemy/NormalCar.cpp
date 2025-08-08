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
	__super::OnStart();
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Tico/tico.png");
	owner->transform()->SetScale(0.7f, 0.7f);
}

#include "JumpTruck.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"
#include "Component/SkewTransform.h"

void JumpTruck::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void JumpTruck::OnStart()
{
	__super::OnStart();

	auto sr = owner->GetComponent<SpriteRenderer>();

	sr->LoadData(L"Enemy/Jumporter/jumporter.png");
	//owner->transform()->SetScale(0.35f, 0.35f);
	
	// 스케일 조정
	FVector2 scale = { 0.35f, 0.35f };
	sr->SetRelativeScale(scale);

	float height = sr->GetBitmapSizeY() * scale.y;
	float yOffset = height * 0.25f;

	// zPos 오프셋 넣기
	SkewTransform* st = owner->GetComponent<SkewTransform>();
	st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));
}

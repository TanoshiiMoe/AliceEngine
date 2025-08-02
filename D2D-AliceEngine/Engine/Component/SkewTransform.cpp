#include "pch.h"
#include "SkewTransform.h"
#include "Manager/UpdateTaskManager.h"
#include <TileMap/TileMapComponent.h>
#include "Define/Define.h"
#include <System/ScriptSystem.h>


void SkewTransform::SetClamp(float minY /*= 0.0f*/, float maxY /*= 0.0f*/)
{
	clamp = std::make_pair(minY, maxY);
}

const FVector2 SkewTransform::GetOffset()
{
	return offset;
}

void SkewTransform::Initialize()
{
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);
}

void SkewTransform::OnStart()
{
	__super::OnStart();

	if (!owner) {
		OutputDebugStringW(L"SkewTransform 컴포넌트의 owner가 없습니다!!!!");
		return;
	}
		

	renderTransform = owner->GetComponent<TransformComponent>();
}

void SkewTransform::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!renderTransform)
		return;

	skewDeg = GetSkew();
	
	// 원래대로 돌려놓기
	renderTransform->AddPosition(-offset.x, -offset.y);

	float rad = Deg2Rad(skewDeg);

	offset.x = -std::sin(rad) * zPos;
	offset.y = std::cos(rad) * zPos;

	// 트랜스폼 적용하기
	renderTransform->AddPosition(offset.x, offset.y);


	//renderTransform->SetPosition(transform.GetPosition() + FVector2(GetOffsetX(), 0.0f));
}

float SkewTransform::GetSkew()
{
	if (groundTile)
		return groundTile->GetComponent<TileMapComponent>()->skewAngle.x;
	
	OutputDebugStringW(L"SkewTransform 컴포넌트에 groundTile 오브젝트가 등록되지 않았습니다!!");
	return 0.0f;
}

float SkewTransform::Deg2Rad(float _deg)
{
	return (_deg / 180.0f) * Define::PI;
}

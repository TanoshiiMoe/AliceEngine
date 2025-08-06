#include "pch.h"
#include "SkewTransform.h"
#include "Manager/UpdateTaskManager.h"
#include <TileMap/TileMapComponent.h>
#include "Define/Define.h"
#include <System/ScriptSystem.h>
#include "Object/gameObject.h"


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

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void SkewTransform::OnStart()
{
	__super::OnStart();

	if (!owner) {
		OutputDebugStringW(L"SkewTransform ������Ʈ�� owner�� �����ϴ�!!!!");
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
	
	// ������ ���� Ʈ������ �����ϱ�
	realPos = renderTransform->GetPosition() - offset;

	// ������� ��������
	renderTransform->AddPosition(-offset.x, -offset.y);

	float rad = Math::Deg2Rad(skewDeg);

	offset.x = -std::sin(rad) * zPos;
	offset.y = std::cos(rad) * zPos;

	// Ʈ������ �����ϱ�
	renderTransform->AddPosition(offset.x, offset.y);

	//renderTransform->SetPosition(transform.GetPosition() + FVector2(GetOffsetX(), 0.0f));
}

float SkewTransform::GetSkew()
{
	if (groundTile)
		return groundTile->GetComponent<TileMapComponent>()->skewAngle.x;
	
	OutputDebugStringW(L"SkewTransform ������Ʈ�� groundTile ������Ʈ�� ��ϵ��� �ʾҽ��ϴ�!!");
	return 0.0f;
}

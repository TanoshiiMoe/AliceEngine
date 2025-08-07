#include "pch.h"
#include "SkewTransform.h"
#include "Manager/UpdateTaskManager.h"
#include <TileMap/TileMapComponent.h>
#include "Define/Define.h"
#include <System/ScriptSystem.h>
#include "Object/gameObject.h"
#include <Animation/AnimatorInstance.h>
#include "Component/SpriteRenderer.h"

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
		OutputDebugStringW(L"SkewTransform ������Ʈ�� owner�� �����ϴ�!!!!\n");
		return;
	}
		

	renderTransform = owner->GetComponent<TransformComponent>();
}

void SkewTransform::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!renderTransform)
		return;

	// ������ ���� Ʈ������ �����ϱ�
	realPos = renderTransform->GetPosition() - offset;

	if (!Math::Approximately(prevzPos, zPos)) {

		// prevPos�� �����ϱ�
		prevzPos = zPos;

		skewDeg = GetSkew();

		// ������� ��������
		renderTransform->AddPosition(-offset.x, -offset.y);

		float rad = Math::Deg2Rad(skewDeg);

		offset.x = -std::sin(rad) * zPos;
		offset.y = std::cos(rad) * zPos;

		// Ʈ������ �����ϱ�
		renderTransform->AddPosition(offset.x, offset.y);

		// ���� ���̾� �����ϱ�
		SetRenderLayer();

		// ������
		std::wstring message = owner->GetName() + L" : Zpos = " + std::to_wstring(zPos) + L"\n";
		OutputDebugStringW(message.c_str());
	}

}

float SkewTransform::GetSkew()
{
	if (groundTile)
		return groundTile->GetComponent<TileMapComponent>()->skewAngle.x;
	
	std::wstring message = owner->GetName() + L" : SkewTransform ������Ʈ�� groundTile ������Ʈ�� ��ϵ��� �ʾҽ��ϴ�!!!\n";

	OutputDebugStringW(message.c_str());
	return 0.0f;
}

void SkewTransform::SetRenderLayer()
{
	if (autoLayer) {
		int layer = static_cast<int>(-zPos);

		if (auto ani = owner->GetComponent<Animator>())
			ani->m_layer = layer;
		else if (auto spr = owner->GetComponent<SpriteRenderer>())
			spr->m_layer = layer;
		else {
			std::wstring message = owner->GetName() + L" : SkewTransform���� Animator�� SpriteRenderer�� ã���� �����ϴ�!!\n";
			OutputDebugStringW(message.c_str());
		}
	}
}

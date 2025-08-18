#include "pch.h"
#include "Collider.h"
#include <System/CollisionSystem.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Component/TransformComponent.h>
#include <Component/BoxComponent.h>
#include <Object/gameObject.h>

Collider::Collider()
{

}

Collider::~Collider()
{
	CollisionSystem::GetInstance().UnRegist(WeakFromThis<Collider>());
}

void Collider::Initialize()
{
	CollisionSystem::GetInstance().Regist(WeakFromThis<Collider>());
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);

	// 기본 충돌용 BoxComponent 생성 및 기본값 설정
	EnsureDebugBox();
	if (m_box)
	{
		m_box->SetUsage(BoxComponent::EUsage::Collision);
		if (m_box->m_size.x == 0.0f && m_box->m_size.y == 0.0f)
		{
			m_box->SetSize(FVector2(50.0f, 50.0f));
		}
		m_box->SetRelativePosition(FVector2(0.0f, 0.0f));
	}
}

void Collider::Update(const float& deltaSeconds)
{
	if (collisionDelay > 0)
	{
		collisionDelay -= deltaSeconds;
		return;
	}
	else if (dirty)
	{
		dirty = false;
		UpdateAABB();
	}

	UpdateAABB();
}

void Collider::SetDebugDraw(bool enable)
{
	m_debugDraw = enable;
	if (m_debugDraw)
		EnsureDebugBox();
	else
	{
		// 디버그 비활성화 시에도 박스는 유지하되, 보이지만 끔
		EnsureDebugBox();
		if (m_box) m_box->SetVisible(false);
	}
}

void Collider::SetDebugRenderLayer(int layer)
{
	m_debugLayer = layer;
	if (m_box) m_box->SetLayer(m_debugLayer);
}

// 호환용: SetLayer는 충돌 채널로 취급
void Collider::SetLayer(const int& _layer)
{
	m_channel = _layer;
}

void Collider::EnsureDebugBox()
{
	if (!m_box)
	{
		if (gameObject* go = GetOwner())
		{
			m_box = go->AddComponent<BoxComponent>();
			m_box->SetName(L"ColliderBox");
			m_box->SetTag(L"__collision_box");
			m_box->SetUsage(BoxComponent::EUsage::Collision);
			m_box->SetIgnoreOwnerScale(false);
			m_box->SetColor(FColor::Red);
			m_box->SetThickness(5.0f);
			m_box->SetDrawType(Define::EDrawType::WorldSpace);
		}
	}
	// 레거시 포인터 항상 동기화
	boxComponent = m_box;
	if (m_box)
	{
		m_box->SetLayer(m_debugLayer);
		SyncDebugBox();
	}
}

void Collider::SyncDebugBox()
{
	if (!m_box) return;
	// 디버그 여부와 무관하게 사이즈는 동기화하여 콘텐츠 코드(boxComponent->SetSize) 경로를 반영
	m_box->SetSize(m_size);
	// position/rotation handled by owner transform + local offset visualization if needed
}

void Collider::SetBoxColor(const FColor _color)
{
	EnsureDebugBox();
	if (m_box)
		m_box->SetColor(_color);
}

FVector2 Collider::GetWorldCenter() const
{
	gameObject* go = owner.Get();
	FVector2 ownerPos = go ? go->transform()->GetPosition() : FVector2(0, 0);
	FVector2 rel = FVector2(0, 0);
	if (boxComponent) rel = boxComponent->GetRelativePosition();
	else if (m_box) rel = m_box->GetRelativePosition();
	else rel = m_localOffset;
	return FVector2(ownerPos.x + rel.x, ownerPos.y + rel.y);
}

void Collider::UpdateAABB()
{
	// 최신 크기/오프셋은 BoxComponent에서 취득
	FVector2 size = m_size;
	if (boxComponent) size = boxComponent->m_size;
	else if (m_box) size = m_box->m_size;

	//FVector2 center = GetWorldCenter();
	FVector2 center = boxComponent ? boxComponent->GetRelativePosition() : GetWorldCenter();
	const float halfW = size.x * 0.5f;
	const float halfH = size.y * 0.5f;
	aabb.minVector.x = center.x - halfW;
	aabb.minVector.y = center.y - halfH;
	aabb.maxVector.x = center.x + halfW;
	aabb.maxVector.y = center.y + halfH;

	SyncDebugBox();
}

void Collider::Release()
{
	// Collider 수명 종료 시 소유 BoxComponent 제거
	if (m_box && GetOwner())
	{
		GetOwner()->RemoveComponent<BoxComponent>(m_box);
		m_box = nullptr;
		boxComponent = nullptr;
	}
}

void Collider::SetBoxSize(const FVector2& _size)
{
	// legacy method kept; redirects to SetSize
	m_size = _size;
	dirty = true;
	EnsureDebugBox();
	SyncDebugBox();
	UpdateAABB();
}

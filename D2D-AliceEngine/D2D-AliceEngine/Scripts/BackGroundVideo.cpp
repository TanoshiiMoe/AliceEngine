#include "BackGroundVideo.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/VideoComponent.h>
#include <System/ScriptSystem.h>

void BackGroundVideo::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);
}

void BackGroundVideo::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	if (!m_player || !m_owner) return;

	FVector2 playerPos = m_player->transform()->GetPosition();
	FVector2 myPos = m_owner->transform()->GetPosition();

	m_owner->transform()->SetPosition(playerPos.x, 550);
	//m_owner->transform()->SetPosition(playerPos.x, Define::SCREEN_HEIGHT);
}

void BackGroundVideo::OnStart()
{
	m_owner = GetOwner();
	m_owner->transform()->SetScale(0.3f, 0.3f);

	m_owner->AddComponent<BackGroundComponent>()->LoadData(L"BackGround\\Mari_Sportswear.webm", 30, L"jpg", 95, true);
	m_owner->GetComponent<BackGroundComponent>()->Play();

	if (m_player != nullptr)
		m_owner->GetComponent<BackGroundComponent>()->SetPlayer(m_player);
}

void BackGroundVideo::OnEnd()
{

}

void BackGroundVideo::SetPlayer(gameObject* player)
{
	m_player = player;
}

#include "GameClearWidgetScript.h"

#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>

void GameClearWidgetScript::Initialize()
{
	__super::Initialize();
}

void GameClearWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void GameClearWidgetScript::Awake()
{
}

void GameClearWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5, 0.5)));

	GetCamera()->AddChildObject(m_owner);

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toMainButton = m_owner->AddComponent<ButtonComponent>();

	// TODO : 
}

void GameClearWidgetScript::OnEnd()
{
}

void GameClearWidgetScript::OnDestroy()
{
}

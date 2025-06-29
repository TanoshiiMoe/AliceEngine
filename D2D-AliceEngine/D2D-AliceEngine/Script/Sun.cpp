#include "Sun.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Manager/SceneManager.h>
void Sun::Initialize()
{
	__super::Initialize();
}

void Sun::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void Sun::OnStart()
{
	m_sun = GetOwner();
	m_sun->transform()->SetPivot(0.5f);
	m_sun->transform()->SetScale(0.5f);
	m_sun->AddComponent<TextRenderComponent>()->SetText(m_sun->GetName());
	m_sun->GetComponent<TextRenderComponent>()->SetTransformType(ETransformType::Unity);
	m_sun->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_sun->GetComponent<TextRenderComponent>()->SetScale(FVector2(2, 2));
	m_sun->AddComponent<SpriteRenderer>()->LoadData(L"Sun.png");
	m_sun->AddComponent<BoxComponent>(m_sun->GetComponent<SpriteRenderer>()->GetSize(), FColor::Red);
	m_sun->GetComponent<TextRenderComponent>()->SetPosition(FVector2(0, -m_sun->GetComponent<SpriteRenderer>()->GetSize().y * 0.7f));

	m_sun->AddComponent<InputComponent>()->SetAction([this]() { Input(); });
}

void Sun::OnEnd()
{
}

void Sun::Input()
{
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
	if (Input::IsKeyPressed(VK_4))
	{
		if (m_sun->GetComponent<TextRenderComponent>()->GetText() == L"Sun")
			m_sun->GetComponent<TextRenderComponent>()->SetText(L"еб╬Г");
		else
			m_sun->GetComponent<TextRenderComponent>()->SetText(L"Sun");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		m_sun->transform()->AddRotation(5.0f);
	}
	if (Input::IsKeyDown(VK_C))
	{
		m_sun->transform()->AddRotation(-5.0f);
	}
	if (Input::IsKeyDown(VK_D))
	{
		m_sun->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_A))
	{
		m_sun->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_W))
	{
		m_sun->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_S))
	{
		m_sun->transform()->AddPosition(0, -5.0f);
	}
}

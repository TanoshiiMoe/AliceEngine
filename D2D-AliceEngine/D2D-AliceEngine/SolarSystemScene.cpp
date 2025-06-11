#include "SolarSystemScene.h"

SolarSystemScene::SolarSystemScene()
{

}

SolarSystemScene::~SolarSystemScene()
{

}

void SolarSystemScene::Initialize()
{
	__super::Initialize();

	m_sun = NewObject<SolarSystemObject>(L"m_sun");
	m_earth = NewObject<SolarSystemObject>(L"m_earth");
	m_moon = NewObject<SolarSystemObject>(L"m_moon");
}

void SolarSystemScene::Release()
{
	__super::Release();
}

void SolarSystemScene::Update()
{
	__super::Update();

	//m_earth->m_bitmapRenderer->m_localTransform->Rotation += 0.5f; // 지구 자전
	//m_moon->m_bitmapRenderer->m_localTransform->Rotation -= 2.0f; // 달 자전
	//m_sun->m_bitmapRenderer->m_localTransform->Rotation += 0.2f;
	//m_sun->Update();
}

void SolarSystemScene::Render()
{
	__super::Render();
}
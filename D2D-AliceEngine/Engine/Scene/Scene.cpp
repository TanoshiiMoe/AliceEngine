#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>

Scene::Scene()
{
	m_mainCamera = nullptr;
}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	GetSingleton(RenderSystem).Initialize();
	for (auto& object : m_objects)
	{
		object.second.get()->Initialize();
	}
	m_mainCamera = new Camera();
	m_mainCamera->Initialize();
}

void Scene::Release()
{
	m_mainCamera->Release();
}

void Scene::Update()
{
	RenderSystem::Get().Update();

	for (auto& object : m_objects)
	{
		object.second.get()->Update();
	}
	
	InputSystem::Get().Update();
}
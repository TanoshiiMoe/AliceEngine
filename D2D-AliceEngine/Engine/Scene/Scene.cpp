#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>

Scene::Scene()
{
	RenderSystem::Get().Initialize();
	for (auto& object : m_objects)
	{
		object.second.get()->Initialize();
	}
	m_mainCamera = nullptr;
	m_mainCamera = new Camera();
	m_mainCamera->Initialize();
}

Scene::~Scene()
{
	m_mainCamera->Release();
}

void Scene::Initialize()
{
	
}

void Scene::Release()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second.reset();
	}
	m_objects.clear();
}

void Scene::Update()
{
	RenderSystem::Get().Update();
	TransformSystem::Get().Update();
	ScriptSystem::Get().Update();
	InputSystem::Get().Update();
}

void Scene::OnEnter()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnStart();
	}
}

void Scene::OnExit()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnEnd();
	}
}


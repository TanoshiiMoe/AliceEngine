#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>
#include <Component/TextRenderComponent.h>
#include <Manager/PackageResourceManager.h>

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
	InputSystem::Get().Update();
	TransformSystem::Get().Update();
	ScriptSystem::Get().Update();

	FMemoryInfo info = PackageResourceManager::Get().GetMemoryInfo();
	m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetText(L"VRAM : " + info.VRAMUssage + L"\n"+ L"DRAM : " + info.DRAMUssage + L"\n" + L"PageFile : " + info.PageFile + L"\n");
}

void Scene::OnEnter()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnStart();
	}

	m_sysinfoWidget = NewObject<gameObject>(L"SystemInfoWidget");
	m_sysinfoWidget->AddComponent<TextRenderComponent>();

	int x = 0, y = 0;
	D2DRenderManager::Get().GetApplicationSize(x, y);
	m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(x * 0.7, y * 0.1));
}

void Scene::OnExit()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnEnd();
	}
}


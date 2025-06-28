#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>
#include <Component/TextRenderComponent.h>
#include <Manager/PackageResourceManager.h>

Scene::Scene()
{
	RenderSystem::GetInstance().Initialize();
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
	RenderSystem::GetInstance().Update();
	InputSystem::GetInstance().Update();
	TransformSystem::GetInstance().Update();
	ScriptSystem::GetInstance().Update();

	FMemoryInfo info = PackageResourceManager::GetInstance().GetMemoryInfo();
	m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetText(L"VRAM : " + info.VRAMUssage + L"\n"+ L"DRAM : " + info.DRAMUssage + L"\n" + L"PageFile : " + info.PageFile + L"\n");
	m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetColor(FColor(200, 0, 0, 255));
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
	D2DRenderManager::GetInstance().GetApplicationSize(x, y);
	m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(x * 0.7, y * 0.1));
}

void Scene::OnExit()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnEnd();
	}
	D2DRenderManager::GetInstance().m_dxgiDevice->Trim();
	PackageResourceManager::GetInstance().UnloadData();
}
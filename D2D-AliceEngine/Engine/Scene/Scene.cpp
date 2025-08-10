#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>
#include <System/RenderSystem.h>
#include <System/CollisionSystem.h>
#include <System/PhysicsSystem.h>
#include <Component/TextRenderComponent.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/ClassManager.h>
#include <Manager/TimerManager.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include <Math/TMath.h>

Scene::Scene()
{
	RenderSystem::GetInstance().Initialize();
	for (auto& object : m_objects)
	{
		if (WeakObjectPtr<gameObject> weak = object.second.get())
		{
			weak->Initialize();
		}
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
	CollisionSystem::GetInstance().Initialize();
	PhysicsSystem::GetInstance().Initialize();
	UpdateTaskManager::GetInstance().SetWorld();
}

void Scene::Release()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second.get()->Release();
		it->second.reset();
	}
	m_objects.clear();
	UpdateTaskManager::GetInstance().ClearWorld();
	CollisionSystem::GetInstance().Release();
	PhysicsSystem::GetInstance().Release();
}

// 첫 프레임에서 ScriptSystem의 Start를 call
void Scene::Update()
{
	UpdateTaskManager::GetInstance().StartFrame();
	UpdateTaskManager::GetInstance().TickAll();
    // F2 토글
    if (Input::IsKeyPressed(VK_F2)) {
        m_debugHudVisible = !m_debugHudVisible;
        if (m_sysinfoWidget) m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetOpacity(m_debugHudVisible ? 1.0f : 0.0f);
        if (m_fpsWidget) m_fpsWidget->GetComponent<TextRenderComponent>()->SetOpacity(m_debugHudVisible ? 1.0f : 0.0f);
    }
    // FPS 갱신 (TimerManager 값을 그대로 사용)
    UpdateDebugHUD(0.0f);
	VisibleMemoryInfo();
	FlushPendingRemovals(); // 프레임 끝에서 삭제 처리
	UpdateTaskManager::GetInstance().EndFrame();
}

void Scene::OnEnter()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second->OnStart();
	}

    m_sysinfoWidget = NewObject<gameObject>(L"SystemInfoWidget");
    //GetCamera()->AddChildObject(m_sysinfoWidget);
    auto* sysText = m_sysinfoWidget->AddComponent<TextRenderComponent>();
    sysText->SetDrawType(Define::EDrawType::ScreenSpace);
    sysText->SetColor(FColor(200, 0, 0, 255));
    // ScreenSpace 좌표 (좌상단 0,0)
    sysText->SetRelativePosition(FVector2(Define::SCREEN_WIDTH * 0.8f, Define::SCREEN_HEIGHT * 0.1f));

    // FPS 위젯
    m_fpsWidget = NewObject<gameObject>(L"FPSWidget");
    //GetCamera()->AddChildObject(m_fpsWidget);
    auto* fpsText = m_fpsWidget->AddComponent<TextRenderComponent>();
    fpsText->SetDrawType(Define::EDrawType::ScreenSpace);
    fpsText->SetColor(FColor(0, 255, 0, 255));
    fpsText->SetRelativePosition(FVector2(Define::SCREEN_WIDTH * 0.8f, Define::SCREEN_HEIGHT * 0.18f));
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

void Scene::VisibleMemoryInfo()
{
    FMemoryInfo info = PackageResourceManager::GetInstance().GetMemoryInfo();
    if (m_sysinfoWidget)
    {
        auto* t = m_sysinfoWidget->GetComponent<TextRenderComponent>();
        t->SetText(L"VRAM : " + info.VRAMUssage + L"\n" + L"DRAM : " + info.DRAMUssage + L"\n" + L"PageFile : " + info.PageFile + L"\n");
    }
}

void Scene::UpdateDebugHUD(float /*deltaTime*/)
{
    if (m_fpsWidget)
    {
        const float fps = TimerManager::GetInstance().GetCurrentFPS();
        auto* t = m_fpsWidget->GetComponent<TextRenderComponent>();
        t->SetText(L"FPS: " + std::to_wstring(static_cast<int>(std::round(fps))));
    }
}

bool Scene::RemoveObject(gameObject* targetObj)
{
	if (!targetObj) return false;
	std::wstring uuid = FindUUIDByPointer(targetObj);
	if (uuid.empty()) return false;

	m_pendingDeleteUUIDs.insert(uuid);
	return true;
}

bool Scene::RemoveObjectByName(const std::wstring& objectName)
{
	auto it = m_nameToUUIDs.find(objectName);
	if (it == m_nameToUUIDs.end() || it->second.empty())
		return false;

	m_pendingDeleteUUIDs.insert(*it->second.begin()); // 하나만 삭제
	return true;
}

bool Scene::RemoveAllObjectsByName(const std::wstring& name)
{
	auto it = m_nameToUUIDs.find(name);
	if (it == m_nameToUUIDs.end() || it->second.empty())
		return false;

	for (const auto& uuid : it->second)
		m_pendingDeleteUUIDs.insert(uuid);
	return true;
}

void Scene::FlushPendingRemovals()
{
	if (m_pendingDeleteUUIDs.empty()) return;

	for (const auto& uuid : m_pendingDeleteUUIDs)
	{
		auto it = m_objects.find(uuid);
		if (it == m_objects.end()) continue;

		const std::wstring& name = it->second->GetName();
		auto nameIt = m_nameToUUIDs.find(name);
		if (nameIt != m_nameToUUIDs.end())
		{
			nameIt->second.erase(uuid);
			if (nameIt->second.empty())
				m_nameToUUIDs.erase(nameIt);
		}

		it->second.reset();
		m_objects.erase(it);
	}
	m_pendingDeleteUUIDs.clear();
}

std::wstring Scene::FindUUIDByPointer(gameObject* ptr) const
{
	if (!ptr) return L"";
	for (const auto& kv : m_objects)
	{
		if (kv.second.get() == ptr)
			return kv.first;
	}
	return L"";
}


gameObject* Scene::Instantiate(gameObject* obj)
{
	gameObject* target = NewObject<gameObject>(obj->GetName());
	//gameObject* target = NewObject<gameObject>(L"replicated name");
	for (UObject* objComp : obj->m_components)
	{
		UObject* createdobj = ClassManager::GetInstance().CreateClass(objComp);
		if (Component* createdComp = dynamic_cast<Component*>(createdobj))
		{
			if (createdComp && objComp)
			{
				if (dynamic_cast<TransformComponent*>(createdComp))
				{
					ClassManager::GetInstance().ReplicateAllMembers(target->m_transformComponent.Get(), objComp);
				}
				else
				{
					ClassManager::GetInstance().ReplicateAllMembers(createdComp, objComp);
				}
				// AddComponent의 로직을 여기에 넣으면 됩니다.
				createdComp->Initialize();
				createdComp->SetOwner(target);
				target->m_components.push_back(createdComp);
			}
		}
	}
	target->transform()->SetDirty();
	return target;
}

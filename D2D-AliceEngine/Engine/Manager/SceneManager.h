#pragma once
#include "Object/Object.h"
#include "Scene/Scene.h"
#include "Singleton.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Initialize();
	virtual void UnInitialize();
	virtual void Update();

	std::weak_ptr<Scene> m_currentScene;	// 현재 씬

	static std::weak_ptr<Scene> GetCurrentScene()
	{
		return Get().m_currentScene;
	}

	static std::weak_ptr<Scene> AddScene(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<Scene> createdObj = std::make_shared<Scene>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = Get().m_scenes.emplace(createdObj->GetUUID(), createdObj);
		auto iter = result.first;

		return std::dynamic_pointer_cast<Scene>(iter->second);
	}

	static std::weak_ptr<Scene> GetUUIDFromObjectName(const std::wstring& name)
	{
		// 이름 기준으로 찾기
		for (auto& scenePair : Get().m_scenes)
		{
			std::weak_ptr<Scene> scene = scenePair.second;
			if (scene.lock()->GetName() == name)
			{
				return scene;
			}
		}
		return std::weak_ptr<Scene>();
	}

	static void ChangeScene(const std::wstring& NewobjectName)
	{
		std::weak_ptr<Scene> searchedScene = GetUUIDFromObjectName(NewobjectName);

		if (searchedScene.lock())
		{
			if (Get().m_currentScene.lock())
			{
				Get().m_currentScene.lock()->Release();
			}
			Get().m_currentScene = searchedScene;
			Get().m_currentScene.lock()->Initialize();
		}
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<Scene>> m_scenes;
};
#define GetSceneManager() SceneManager::Get()
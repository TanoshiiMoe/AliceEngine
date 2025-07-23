#include "pch.h"
#include "SceneManager.h"
#include <Scene/Scene.h>

SceneManager::SceneManager() : m_currentScene{nullptr}
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{

}

void SceneManager::UnInitialize()
{

}

void SceneManager::Update()
{
    if (!m_nextSceneName.empty())
    {
        PerformSceneChange(m_nextSceneName);
        m_nextSceneName.clear();
    }

    if (WeakObjectPtr<Scene> scene = m_currentScene)
        scene->Update();

    GetCamera()->Update();
}

Scene* SceneManager::GetWorld()
{
    if (WeakObjectPtr<Scene> scene = m_currentScene)
    {
        return scene.Get();
    }
    else
    {
        return nullptr;
    }
}

Camera* SceneManager::GetCamera()
{
    if (WeakObjectPtr<Scene> scene = GetInstance().m_currentScene)
    {
        return scene->GetCamera();
    }
    return nullptr;
}

void SceneManager::PerformSceneChange(const std::wstring& NewobjectName)
{
    if (WeakObjectPtr<Scene> searchedScene = GetWeakPtrFromObjectName(NewobjectName))
    {
        if (WeakObjectPtr<Scene> weak = m_currentScene)	// ���� ���� �ִٸ� �� ���� Exit ��ŵ�ϴ�.
        {
            weak->OnExit();
            weak->Release();
            SceneManager::GetInstance().GetCamera()->Release();
        }
        m_currentScene = searchedScene.Get();
        m_currentScene->Initialize();
        m_currentScene->OnEnter();	// �ٲٷ��� ���� OnEnter() �Լ��� �����ŵ�ϴ�.
        SceneManager::GetInstance().GetCamera()->Initialize();
    }
}

WeakObjectPtr<Scene> SceneManager::GetWeakPtrFromObjectName(const std::wstring& name)
{
	// �̸� �������� ã��
	for (auto& scenePair : GetInstance().m_scenes)
	{
		if (WeakObjectPtr<Scene> sceneWeak = scenePair.second.get())
		{
			if (sceneWeak->GetName() == name)
			{
				return sceneWeak;
			}
		}
	}
	return nullptr;
}

void SceneManager::ChangeScene(const std::wstring& NewobjectName)
{
    GetInstance().m_nextSceneName = NewobjectName;
}

void SceneManager::RestartScene()
{
    GetInstance().m_nextSceneName = GetInstance().m_currentScene->GetName();
}

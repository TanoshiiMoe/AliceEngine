#include "pch.h"
#include "SceneManager.h"
#include <Scene/Scene.h>

SceneManager::SceneManager()
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

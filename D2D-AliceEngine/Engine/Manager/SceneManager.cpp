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

    if (auto scene = m_currentScene.lock())
        scene->Update();

    GetCamera()->Update();
}

Scene* SceneManager::GetWorld()
{
    if (std::shared_ptr<Scene> sharedScene = m_currentScene.lock())
    {
        return sharedScene.get();
    }
    else
    {
        return nullptr;
    }
}

Camera* SceneManager::GetCamera()
{
    if (GetInstance().m_currentScene.lock())
    {
        return GetInstance().m_currentScene.lock()->GetCamera();
    }
    return nullptr;
}

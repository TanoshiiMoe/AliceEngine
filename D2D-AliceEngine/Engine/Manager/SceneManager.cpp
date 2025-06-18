#include "pch.h"
#include "SceneManager.h"

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
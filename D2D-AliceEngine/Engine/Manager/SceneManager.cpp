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
	if(m_currentScene.lock())
	{
		m_currentScene.lock()->Update();
	}
}
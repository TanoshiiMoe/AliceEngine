#include "pch.h"
#include "ScriptComponent.h"
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Scene/Scene.h>

ScriptComponent::ScriptComponent()
{

}

ScriptComponent::~ScriptComponent()
{
	ScriptSystem::GetInstance().UnRegist(this->weak_from_this());
}

void ScriptComponent::Initialize()
{
	ScriptSystem::GetInstance().Regist(this->weak_from_this());
}

void ScriptComponent::Release()
{
	
}

Scene* ScriptComponent::GetWorld()
{
	return SceneManager::GetInstance().GetWorld();
}

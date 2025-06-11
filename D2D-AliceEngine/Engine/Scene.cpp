#include "pch.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{

}

void Scene::Initialize()
{

}

void Scene::Release()
{

}

void Scene::Update()
{
	for (auto& component : m_objects)
	{
		component.second.get()->Update();
	}
}

void Scene::Render()
{
	for (auto& component : m_objects)
	{
		component.second.get()->Render();
	}
}
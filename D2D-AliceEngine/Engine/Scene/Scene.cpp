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
	for (auto& object : m_objects)
	{
		object.second.get()->Initialize();
	}
}

void Scene::Release()
{

}

void Scene::Update()
{
	for (auto& object : m_objects)
	{
		object.second.get()->Update();
	}
}
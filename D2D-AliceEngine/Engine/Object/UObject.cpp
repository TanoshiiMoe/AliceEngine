#include "pch.h"
#include "UObject.h"
#include <Scene/Scene.h>
#include <Manager/SceneManager.h>

UObject::UObject()
{
	objectHandle = ObjectHandler::GetInstance().CreateHandle(this);
}

UObject::~UObject()
{
	ObjectHandler::GetInstance().DestroyHandle(objectHandle);
}

Scene* UObject::GetWorld()
{
	if (Scene* scene = SceneManager::GetInstance().GetWorld())
	{
		return scene; // ���尡 ���� ��� nullptr ��ȯ
	}
	return nullptr;
}

#include "pch.h"
#include "UObject.h"

UObject::UObject()
{
	objectHandle = ObjectHandler::GetInstance().CreateHandle(this);
}

UObject::~UObject()
{
	ObjectHandler::GetInstance().DestroyHandle(objectHandle);
}
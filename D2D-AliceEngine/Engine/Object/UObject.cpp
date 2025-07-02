#include "pch.h"
#include "UObject.h"

UObject::UObject()
{
	ObjectHandler::GetInstance().Register(this);
}

UObject::~UObject()
{
	ObjectHandler::GetInstance().Unregister(this);
}
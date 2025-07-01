#include "pch.h"
#include "UObject.h"

UObject::UObject()
{
	m_handle = ObjectHandler::GetInstance().CreateHandle(this);
}

UObject::~UObject()
{
	ObjectHandler::GetInstance().DestroyHandle(m_handle);
}

ObjectHandle UObject::GetHandle()
{
	return m_handle;
}

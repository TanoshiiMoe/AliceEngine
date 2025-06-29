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

std::wstring& UObject::GetName()
{
	return m_objectName;
}

void UObject::SetName(const std::wstring& value)
{
	m_objectName = value;
}

std::wstring& UObject::GetUUID()
{
	return m_uuid;
}

void UObject::SetUUID(const std::wstring& value)
{
	m_uuid = value;
}

ObjectHandle UObject::GetHandle()
{
	return m_handle;
}

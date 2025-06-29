#pragma once
#include <string>
#include <Core/ObjectHandler.h>
#include <Core/Singleton.h>
class UObject
{
	ObjectHandle m_handle;
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;

public:
	std::wstring& GetName();
	void SetName(const std::wstring& value);

	std::wstring& GetUUID();
	void SetUUID(const std::wstring& value);
	ObjectHandle GetHandle();

protected:
	std::wstring m_objectName;
	std::wstring m_uuid;
};


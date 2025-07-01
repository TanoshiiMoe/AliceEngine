#pragma once
#include <string>
#include <Core/ObjectHandler.h>
#include <Core/Singleton.h>
#include <Object/UObjectBase.h>

class UObject : public UObjectBase
{
	ObjectHandle m_handle;
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;

	ObjectHandle GetHandle();
};


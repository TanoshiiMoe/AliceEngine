#pragma once
#include <string>
#include <Core/ObjectHandler.h>
#include <Core/Singleton.h>
#include <Object/UObjectBase.h>

class UObject : public UObjectBase
{
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
	ObjectHandle GetHandle() const { return objectHandle; }

	template<typename T>
	WeakObjectPtr<T> WeakFromThis()
	{
		// T*로 안전하게 캐스팅해서 WeakObjectPtr 생성
		return WeakObjectPtr<T>(dynamic_cast<T*>(this));
	}

protected:
	ObjectHandle objectHandle;
};


#pragma once
#include "../pch.h"
#include "../Transform.h"
#include "../BitmapRenderer.h"
#include "../Manager/ComponentManager.h"
#include "../Interface/IObject.h"

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class Object : public std::enable_shared_from_this<Object>, public IObject
{
public:
	Object()
	{}
	Object(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
	{
		Initialize(path, position, rotation, scale, pivot);
	}
	virtual ~Object()
	{
	}

	template<class T> 
	std::weak_ptr<T> AddComponent(const std::wstring& componentName)
	{
		return CreateDefaultSubobject(T, componentName);
	}

	template<class T>
	std::weak_ptr<T> GetComponent(const std::wstring& componentName)
	{
		return CreateDefaultSubobject(T, componentName);
	}
	
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Render() override;

	virtual void Initialize(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f));

	void LoadData(const std::wstring& path);
};
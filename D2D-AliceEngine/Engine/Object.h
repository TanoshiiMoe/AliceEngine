#pragma once
#include "pch.h"
#include "Transform.h"
#include "BitmapRenderer.h"

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class Object : public std::enable_shared_from_this<Object>
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
		if (m_bitmapRenderer)
		{
			m_bitmapRenderer.reset();
		}
	}

	virtual void Initialize();
	virtual void Initialize(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f));
	virtual void Update();
	virtual void Release();
	virtual void Render();

	void LoadData(const std::wstring& path);

public:
	std::shared_ptr<BitmapRenderer> m_bitmapRenderer;

	inline std::weak_ptr<BitmapRenderer> GetRenderer()
	{
		return m_bitmapRenderer;
	}
};
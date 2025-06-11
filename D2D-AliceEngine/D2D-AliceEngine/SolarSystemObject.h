#pragma once
#include <Object/Object.h>

class SolarSystemObject : public Object
{
public:
	SolarSystemObject()
	{
	}
	SolarSystemObject(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
	{
		Initialize(path, position, rotation, scale, pivot);
	}
	virtual ~SolarSystemObject()
	{
		if (m_bitmapRenderer)
		{
			m_bitmapRenderer.reset();
		}
	}

	void Initialize() override;
	void Initialize(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f));
	void Update();
	void Release();
	void Render();

	void LoadData(const std::wstring& path);

public:
	std::weak_ptr<BitmapRenderer> m_bitmapRenderer;

	inline std::weak_ptr<BitmapRenderer> GetRenderer()
	{
		return m_bitmapRenderer;
	}
};


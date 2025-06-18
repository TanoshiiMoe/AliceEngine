#pragma once

#include "pch.h"
#include "Component/RenderComponent.h"
#include "Object/gameObject.h"

enum class ETransformType
{
	D2D,
	Unity,
	Max
};

class D2DRenderManager : public Singleton<D2DRenderManager>
{
public:
	D2DRenderManager();
	~D2DRenderManager();

	// 렌더링 대기열
	std::vector<std::weak_ptr<RenderComponent>> m_spriteRenderers;
	std::vector<std::weak_ptr<RenderComponent>> m_boxComponentRenders;

	inline void AddRenderer(std::weak_ptr<RenderComponent> renderer)
	{
		m_spriteRenderers.push_back(renderer);
	}

	inline static ID2D1DeviceContext7* GetD2DDevice()
	{
		return Get().m_d2dDeviceContext.Get();
	}

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void UnInitialize();
	void Render();

	void DrawBoxComponent();		// Widget으로 빼야하는 기능
	void DrawSpriteRenderer();	// m_containerRenders를 모두 그리기

	void GetApplicationSize(int& width, int& height);

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	void CreateSwapChainAndD2DTarget();	// 비트맵을 여러 개 묶어주는 함수
	void OutputError(HRESULT hr);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// For DrawText
	ComPtr<IDWriteFactory> m_dWriteFactory;
	// For ImageDraw
	ComPtr<IWICImagingFactory> m_wicImagingFactory;
	// SpriteBatch
	ComPtr<ID2D1SpriteBatch>    g_spriteBatch;
	bool m_resizePending = false;

	// Transform Type
	ETransformType m_eTransformType = ETransformType::Unity;
	void SwitchTransformType(ETransformType _type)
	{
		m_eTransformType = _type;
	}
};
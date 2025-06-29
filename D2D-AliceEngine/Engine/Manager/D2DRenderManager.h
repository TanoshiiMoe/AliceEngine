#pragma once
#include "pch.h"
#include <Core/Singleton.h>

class Component;
using namespace Define;
class D2DRenderManager : public Singleton<D2DRenderManager>
{
public:
	D2DRenderManager();
	~D2DRenderManager();

	// 렌더링 대기열
	std::vector<std::vector<std::weak_ptr<Component>>> m_renderers;

	void AddRenderer(std::weak_ptr<Component> renderer);

	static ID2D1DeviceContext7* GetD2DDevice();

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void UnInitialize();
	void Render();

	void GetApplicationSize(int& width, int& height);

	void CreateSwapChainAndD2DTarget();	// 비트맵을 여러 개 묶어주는 함수
	void OutputError(HRESULT hr);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// For MemoryInfo
	ComPtr<IDXGIAdapter3> m_dxgiAdapter;
	ComPtr<IDXGIDevice3> m_dxgiDevice;

	// For DrawText
	ComPtr<IDWriteFactory> m_dWriteFactory;
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
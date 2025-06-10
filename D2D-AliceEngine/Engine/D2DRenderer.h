#pragma once

#include "pch.h"
#include "Object.h"

using namespace Microsoft::WRL;
enum class ETransformType
{
	D2D,
	Unity,
	Max
};

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();
	// D2D 렌더링을 위한 초기화 및 해제

	// 렌더링 대기열
	DoubleLinkedList<std::weak_ptr<Object>> m_renderList;

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void Uninitialize();
	void Render();

	void DrawTestText();
	void DrawInRenderList();

	void GetApplicationSize(int& width, int& height);

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	void CreateSwapChainAndD2DTarget();	// 비트맵을 여러 개 묶어주는 함수
	void OutputError(HRESULT hr);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;
	// For BrushAndShape
	ComPtr<ID2D1SolidColorBrush> m_pRedBrush;		// 렌더타겟이 생성하는 리소스 역시 장치의존

	// For DrawText
	ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	ComPtr<IDWriteFactory> m_dWriteFactory;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;
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


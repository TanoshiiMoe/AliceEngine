#pragma once
#include "pch.h"
#include <Core/Singleton.h>
#include <d2d1effectauthor.h>

/*
* @briefs : 
	������ �ʿ��� ������ �Լ����� ���ִ� �̱��� �Ŵ����Դϴ�.
	RenderSystem���� �������մϴ�.
*/

using namespace Define;
class D2DRenderManager : public Singleton<D2DRenderManager>
{
public:
	D2DRenderManager();
	~D2DRenderManager();

	static ID2D1DeviceContext7* GetD2DDevice();

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void UnInitialize();

	HWND GetHWND() { return m_hwnd; }
	FVector2 GetApplicationSize();
	void CreateSwapChainAndD2DTarget();	// ��Ʈ���� ���� �� �����ִ� �Լ�
	void OutputError(HRESULT hr);
	void DrawDebugBox(const float& startPosX, const float& startPosY, const float& ensPosX, const float& ensPosY, const float& r, const float& g, const float& b, const float& a);
	void DrawDebugText(const std::wstring& text, float posX, float posY, float fontSize, const D2D1::ColorF& color);

	void CreateAfterEffectScreenRenderTarget();

	void LoadGradientTextrue();
	void LoadEffectShader();

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;
	ComPtr<ID2D1Bitmap1> m_screenBitmap;
	ComPtr<IWICImagingFactory> m_wicFactory;

	// For MemoryInfo
	ComPtr<IDXGIAdapter3> m_dxgiAdapter;
	ComPtr<IDXGIDevice3> m_dxgiDevice;

	// For DrawText
	ComPtr<IDWriteFactory> m_dWriteFactory;
	// SpriteBatch
	ComPtr<ID2D1SpriteBatch> m_spriteBatch;
	bool m_resizePending = false;

	// Debug�� box
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	// ��ó����
	ComPtr<ID2D1Bitmap1> m_overlayBitmap; // �׶��̼� or �ؽ�ó
	ComPtr<ID2D1Effect> m_sceneEffect;

	// Transform Type
	ETransformType m_eTransformType = ETransformType::Unity;
	void SwitchTransformType(ETransformType _type)
	{
		m_eTransformType = _type;
	}
};
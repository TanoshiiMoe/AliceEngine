#pragma once
#include "pch.h"
#include <Core/Singleton.h>
#include <d2d1effectauthor.h>
#include <d2d1_3.h>
#include <d2d1effects.h>
#include <d2d1effecthelpers.h>

/*
* @briefs : 
	렌더에 필요한 변수와 함수들이 모여있는 싱글톤 매니저입니다.
	RenderSystem에서 렌더링합니다.
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
	void CreateSwapChainAndD2DTarget();	// 비트맵을 여러 개 묶어주는 함수
	void OutputError(HRESULT hr);
	void DrawDebugBox(const float& startPosX, const float& startPosY, const float& ensPosX, const float& ensPosY, const float& r, const float& g, const float& b, const float& a);
	void DrawDebugText(const std::wstring& text, float posX, float posY, float fontSize, const D2D1::ColorF& color);

	void CreateAfterEffectScreenRenderTarget();

	void LoadGradientTextrue();
	void LoadEffectShader();
	
	// 고급 쉐이더 효과 메서드들
	void SetupAdvancedShaderEffects();
	void CreateShaderChain();
	void ApplyShaderEffect(ID2D1Effect* effect, ID2D1Bitmap1* input, ID2D1Bitmap1* output);
	
	// 개별 쉐이더 효과들
	void CreateColorMatrixEffect();
	void CreateGaussianBlurEffect();
	void CreateBlendEffect();
	void CreateTurbulenceEffect();
	void CreateMorphologyEffect();
	void CreateDisplacementEffect();
	void CreateArithmeticComposite();

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_bitmapTarget;
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

	// Debug용 box
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	// 후처리용
	ComPtr<ID2D1Bitmap1> m_overlayBitmap; // 그라데이션 or 텍스처
	ComPtr<ID2D1Effect> m_sceneEffect;
	
	// 고급 쉐이더 효과들
	ComPtr<ID2D1Effect> m_colorMatrixEffect;      // 색상 변환
	ComPtr<ID2D1Effect> m_gaussianBlurEffect;     // 가우시안 블러
	ComPtr<ID2D1Effect> m_blendEffect;            // 블렌딩
	ComPtr<ID2D1Effect> m_turbulenceEffect;       // 터빌런스
	ComPtr<ID2D1Effect> m_morphologyEffect;       // 모폴로지
	ComPtr<ID2D1Effect> m_displacementEffect;     // 변위
	ComPtr<ID2D1Effect> m_arithmeticComposite;    // 산술 합성
	
	// 쉐이더 효과 체인
	ComPtr<ID2D1Effect> m_shaderChain;            // 쉐이더 체인
	bool m_useAdvancedShaders = false;             // 고급 쉐이더 사용 여부

	// Transform Type
	ETransformType m_eTransformType = ETransformType::Unity;
	void SwitchTransformType(ETransformType _type)
	{
		m_eTransformType = _type;
	}

	bool bRenderedBoxRect = true;
};

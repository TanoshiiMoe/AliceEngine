#pragma once
#include "pch.h"
#include "D2DRenderManager.h"
#include <Manager/SceneManager.h>
#include <Define/Define.h>
#include <Helpers/FileHelper.h>
#include <d2d1_3.h>            // ID2D1DeviceContext2, ID2D1Factory2 ��
#include <d2d1effectauthor.h>  // LoadPixelShader ����
#include <d2d1effecthelpers.h> // ���̴� ����
#include <Manager/PackageResourceManager.h>

D2DRenderManager::D2DRenderManager()
{
	
}

D2DRenderManager::~D2DRenderManager()
{
	
}

ID2D1DeviceContext7* D2DRenderManager::GetD2DDevice()
{
	return GetInstance().m_d2dDeviceContext.Get();
}

void D2DRenderManager::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	// D3D11 ����̽� ����
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL obtainedLevel;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.GetAddressOf(),
		&obtainedLevel,
		nullptr
	);
	assert(SUCCEEDED(hr));

	hr = CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicFactory)
	);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// D2D ���丮 �� ����̽�
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	m_d3dDevice.As(&m_dxgiDevice);
	m_dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	dxgiAdapter.As(&m_dxgiAdapter);

	m_d3dDevice.As(&m_dxgiDevice);

	ComPtr<ID2D1Device7> d2dDevice;
	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.GetAddressOf());
	assert(SUCCEEDED(hr));

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain ����
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = GetApplicationSize().x;
	scDesc.Height = GetApplicationSize().y;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd(
		m_d3dDevice.Get(),
		hwnd,
		&scDesc,
		nullptr,
		nullptr,
		m_dxgiSwapChain.GetAddressOf()
	);
	assert(SUCCEEDED(hr));

	// ����۸� Ÿ������ ����
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_screenBitmap.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_screenBitmap.Get());

	// DirectWrite ���͸��� ����ϴ�.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf()));

	// 4. D2D Ÿ�� ��Ʈ�� �� SetTarget
	CreateSwapChainAndD2DTarget();
	CreateAfterEffectScreenRenderTarget();

	// 7. SpriteBatch ����
	hr = m_d2dDeviceContext->CreateSpriteBatch(m_spriteBatch.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 255), m_pBrush.GetAddressOf());

	hr = CreateBitmapFromFile(L"../Resource/BackGround/BG_CS_Arona_04.png", m_overlayBitmap.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D1Saturation, m_sceneEffect.GetAddressOf());
	m_sceneEffect->SetInput(0, m_screenBitmap.Get());     // ���� ȭ��
	m_sceneEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 10.0f);

}

void D2DRenderManager::UnInitialize()
{
	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_screenBitmap = nullptr;

	// For DrawText
	m_dWriteFactory = nullptr;
	m_spriteBatch = nullptr;
}

FVector2 D2DRenderManager::GetApplicationSize()
{
	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	float width = (float)(rc.right - rc.left);
	float height = (float)(rc.bottom - rc.top);
	return { width, height };
}

void D2DRenderManager::CreateSwapChainAndD2DTarget()
{
	// 1. ���� Ÿ�� ����
	if (m_d2dDeviceContext)
	{
		m_d2dDeviceContext->SetTarget(nullptr);
	}
	m_screenBitmap.Reset();

	// 2. SwapChain �缳��
	if (m_dxgiSwapChain) {
		// ResizeBuffers: ����/���̰� 0�̸� ���� ���� ����, ���⼱ ���� ũ��� �ѱ�ϴ�.
		m_dxgiSwapChain->ResizeBuffers(
			0,
			GetApplicationSize().x,
			GetApplicationSize().y,
			DXGI_FORMAT_UNKNOWN,
			0
		);
	}

	// 3. ����� IDXGISurface ���
	ComPtr<IDXGISurface> backBuffer;
	HRESULT hr = m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 4. D2D1BitmapProperties1 ���� 
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// 5. DeviceContext�� ���ε��� ID2D1Bitmap1 ����
	hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, &m_screenBitmap);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 6. ���ؽ�Ʈ�� SetTarget
	m_d2dDeviceContext->SetTarget(m_screenBitmap.Get());
}

void D2DRenderManager::OutputError(HRESULT hr)
{
	_com_error err(hr);
	OutputDebugString(err.ErrorMessage());
}

void D2DRenderManager::DrawDebugBox(const float& startPosX, const float& startPosY, const float& ensPosX, const float& ensPosY, const float& r, const float& g, const float& b, const float& a)
{
	m_pBrush->SetColor(D2D1::ColorF(r, g, b, a));
	m_d2dDeviceContext->DrawRectangle(D2D1::RectF(startPosX, startPosY, ensPosX, ensPosY), m_pBrush.Get(), 3.0f);
}

void D2DRenderManager::DrawDebugText(
	const std::wstring& text,
	float posX,
	float posY,
	float fontSize,
	const D2D1::ColorF& color)
{
	if (!m_d2dDeviceContext || !m_dWriteFactory)
		return;

	// 1. �귯�� ���� ����
	m_pBrush->SetColor(color);

	// 2. �ؽ�Ʈ ���� ����
	ComPtr<IDWriteTextFormat> textFormat;
	HRESULT hr = m_dWriteFactory->CreateTextFormat(
		L"���� ���",              // ��Ʈ
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ko-kr",
		&textFormat
	);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 3. ��� ���� ����
	D2D1_RECT_F layoutRect = D2D1::RectF(posX, posY, posX + 1000.0f, posY + fontSize + 10.0f);

	// 4. �׸���
	m_d2dDeviceContext->DrawTextW(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		textFormat.Get(),
		&layoutRect,
		m_pBrush.Get()
	);
}

void D2DRenderManager::CreateAfterEffectScreenRenderTarget()
{
	D2D1_BITMAP_PROPERTIES1 bmpProps =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	FVector2 size = GetApplicationSize();

	m_d2dDeviceContext->CreateBitmap(
		D2D1::SizeU((UINT32)size.x, (UINT32)size.y),
		nullptr,
		0,
		&bmpProps,
		&m_overlayBitmap);
}

void D2DRenderManager::LoadGradientTextrue()
{
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter> converter;

	m_wicFactory->CreateDecoderFromFilename(
		L"Resources/gradient.png", nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);

	decoder->GetFrame(0, &frame);

	m_wicFactory->CreateFormatConverter(&converter);
	converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

	m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), nullptr, &m_overlayBitmap);
}

void D2DRenderManager::LoadEffectShader()
{
	//std::vector<BYTE> shaderBytes = FileHelper::LoadBinaryFile(L"MyShader.cso");
	//GUID myShaderGUID = { /* ������ GUID */ };
	//
	//ComPtr<ID2D1DeviceContext4> context4;
	//m_d2dDeviceContext.As(&context4);
	//
	//context4->LoadPixelShader(
	//	myShaderGUID,
	//	shaderBytes.data(),
	//	(UINT32)shaderBytes.size()
	//);
	//
	//m_d2dDeviceContext->CreateEffect(CLSID_D2D1DrawPixelShader, &shaderEffect);
	//shaderEffect->SetValue(D2D1_DRAW_PIXEL_SHADER_PROP_SHADER_GUID, myShaderGUID);
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;

	// �� ���ڴ� ����
	HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// �� ù ������ ���
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// �� ���� ��ȯ�� ����
	hr = m_wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// �� GUID_WICPixelFormat32bppPBGRA�� ��ȯ
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return hr;

	// �� Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// �� DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
}
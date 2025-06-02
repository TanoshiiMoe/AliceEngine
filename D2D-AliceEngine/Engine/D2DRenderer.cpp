#pragma once
#include "pch.h"
#include "D2DRenderer.h"

D2DRenderer::D2DRenderer()
{
}

D2DRenderer::~D2DRenderer()
{
}

void D2DRenderer::Initialize(HWND hwnd, int width, int height)
{
	HRESULT hr;
	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;
	d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;
	scDesc.Height = height;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, m_dxgiSwapChain.GetAddressOf());

	// 백버퍼를 타겟으로 설정
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());


	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf());

	hr = CreateBitmapFromFile(L"../Resource/mushroom.png", m_d2dBitmapFromFile.GetAddressOf());
	assert(SUCCEEDED(hr));
}


void D2DRenderer::Uninitialize()
{
	m_wicImagingFactory = nullptr;
	m_d2dBitmapFromFile = nullptr;

	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_d2dBitmapTarget = nullptr;
}

void D2DRenderer::Render()
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	D2D1_SIZE_F size;

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화

	//1. 0,0 위치에 비트맵 전체영역 그린다. (변환은 초기화)
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get());

	//2. DestPos(화면 위치) 설정과 SrcPos(비트맵 위치)로 그리기
	D2D1_VECTOR_2F DestPos{ 0,0 }, SrcPos{ 0,0 }; // 화면 위치, 비트맵 위치
	size = { 0,0 };	//	그릴 크기
	D2D1_RECT_F DestRect{ 0,0,0,0 }, SrcRect{ 0,0,0,0 }; // 화면 영역, 비트맵 영역
	D2D1_MATRIX_3X2_F transform;	// 변환 행렬

	size = m_d2dBitmapFromFile->GetSize();
	DestPos = { 100,0 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect);


	//3. DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	size = { 200,200 };
	DestPos = { 100,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	SrcPos = { 0,0 };
	SrcRect = { SrcPos.x,SrcPos.y, SrcPos.x + size.width - 1 ,SrcPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);


	//4. 변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 700,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(-1.0f, 1.0f,  // x축 반전
		D2D1::Point2F(DestPos.x, DestPos.y));        // 기준점
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	//5. 복합변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 0,0 };   // 그릴 위치는 0,0으로 하고 이동변환을 사용한다.
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Rotation(90.0f, D2D1::Point2F(0.0f, 0.0f)) * // 90도 회전
		D2D1::Matrix3x2F::Translation(900.0f, 900.0f);  // 이동

	// 기준점
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);
}

HRESULT D2DRenderer::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;

	// ① 디코더 생성
	HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// ② 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// ③ 포맷 변환기 생성
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// ④ GUID_WICPixelFormat32bppPBGRA로 변환
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return hr;

	// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
}
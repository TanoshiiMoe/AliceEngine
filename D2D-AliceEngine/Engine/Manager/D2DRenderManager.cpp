#pragma once
#include "pch.h"
#include "D2DRenderManager.h"
#include "Component/TextRenderComponent.h"
#include <Component/SpriteRenderer.h>
#include <Component/VideoComponent.h>
#include <Component/BoxComponent.h>
#include <Component/Animator.h>
#include <Manager/SceneManager.h>
#include <Define/Define.h>

D2DRenderManager::D2DRenderManager()
{
	m_renderers.assign(static_cast<int>(ERenderLayer::Max), std::vector<WeakObjectPtr<RenderComponent>>());
}

D2DRenderManager::~D2DRenderManager()
{
	m_renderers.clear();
}

void D2DRenderManager::AddRenderer(WeakObjectPtr<RenderComponent> renderer)
{
	if (!renderer.expired())
	{
		auto ptr = renderer.lock(); // raw pointer 반환

		if (dynamic_cast<SpriteRenderer*>(ptr))
		{
			m_renderers[static_cast<int>(ERenderLayer::SpriteComponent)].push_back(renderer);
		}
		else if (dynamic_cast<VideoComponent*>(ptr))
		{
			m_renderers[static_cast<int>(ERenderLayer::VideoComponent)].push_back(renderer);
		}
		else if (dynamic_cast<BoxComponent*>(ptr))
		{
			m_renderers[static_cast<int>(ERenderLayer::BoxComponent)].push_back(renderer);
		}
		else if (dynamic_cast<TextRenderComponent*>(ptr))
		{
			m_renderers[static_cast<int>(ERenderLayer::TextRenderComponent)].push_back(renderer);
		}
		else if (dynamic_cast<Animator*>(ptr))
		{
			m_renderers[static_cast<int>(ERenderLayer::Animator)].push_back(renderer);
		}
	}
}

ID2D1DeviceContext7* D2DRenderManager::GetD2DDevice()
{
	return GetInstance().m_d2dDeviceContext.Get();
}

void D2DRenderManager::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	// D3D11 디바이스 생성
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

	// D2D 팩토리 및 디바이스
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

	int width{ 0 }, height{ 0 };
	GetApplicationSize(width, height);

	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;
	scDesc.Height = height;
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

	// 백버퍼를 타겟으로 설정
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	// DirectWrite 팩터리를 만듭니다.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf()));

	// 4. D2D 타겟 비트맵 및 SetTarget
	CreateSwapChainAndD2DTarget();

	// 7. SpriteBatch 생성
	hr = m_d2dDeviceContext->CreateSpriteBatch(g_spriteBatch.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 255), m_pBrush.GetAddressOf());
}

void D2DRenderManager::UnInitialize()
{
	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_d2dBitmapTarget = nullptr;

	// For DrawText
	m_dWriteFactory = nullptr;
	g_spriteBatch = nullptr;
	m_renderers.clear();
}

void D2DRenderManager::Render()
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
	if (m_resizePending)
	{
		CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	ViewRect view = GetCameraView();
	std::vector<WeakObjectPtr<RenderComponent>> collectedComponents;
	for (int i = 0 ; i < static_cast<int>(Define::ERenderLayer::Max); i++)
	{
		if (m_renderers[i].empty()) continue;

		sort(m_renderers[i].begin(), m_renderers[i].end(), &D2DRenderManager::RenderSortCompare);

		for(auto it = m_renderers[i].begin(); it != m_renderers[i].end(); )
		{
			if (it->expired())
			{
				it = m_renderers[i].erase(it);
				continue;
			}
			if (it->lock()->drawType == Define::EDrawType::WorldSpace && CheckCameraCulling(*it, view)) {
				++it;
				continue;
			}
			auto renderer = dynamic_cast<RenderComponent*>(it->lock());
			if (renderer->m_layer == -999)
			{
				collectedComponents.push_back(renderer); 
			}
			else
			{
				it->lock()->Render();
			}
			++it;
		}
	}
	sort(collectedComponents.begin(), collectedComponents.end(), &D2DRenderManager::RenderSortCompare);
	for (auto it = collectedComponents.begin(); it != collectedComponents.end(); ++it)
	{
		if (!it->expired())
		{
			it->lock()->Render();
		}
	}

	HRESULT hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		OutputError(hr);
	}

	m_dxgiSwapChain->Present(1, 0);
}

ViewRect D2DRenderManager::GetCameraView()
{
	const float camX = SceneManager::GetCamera()->GetPositionX();
	const float camY = SceneManager::GetCamera()->GetPositionY();
	const float fov = SceneManager::GetCamera()->fieldOfView;
	FVector2 scale = SceneManager::GetCamera()->GetScale();
	if (scale.x == 0.0f) scale.x = 1.0f;
	if (scale.y == 0.0f) scale.y = 1.0f;
	int screenWidth = 0, screenHeight = 0;
	D2DRenderManager::GetApplicationSize(screenWidth, screenHeight);
	const float halfWidth = (screenWidth * 0.5f * fov) * scale.x;
	const float halfHeight = (screenHeight * 0.5f * fov) * scale.y;
	return ViewRect {camX - halfWidth, camX + halfWidth, camY - halfHeight, camY + halfHeight};
}

// 컬링이 되는걸 확실하게 보려면 주석처리 된 부분을 사용
bool D2DRenderManager::CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view)
{
	auto* transform = renderer->GetTransform();
	const auto pos = transform ? transform->GetPosition() : D2D1_VECTOR_2F{ 0, 0 };
	const auto scale = transform ? transform->GetScale() : D2D1_VECTOR_2F{ 1, 1 };

	/*const float shrinkRatio = 0.5;
    const float halfW = renderer->GetSizeX() * 0.5f * scale.x * shrinkRatio;
    const float halfH = renderer->GetSizeY() * 0.5f * scale.y * shrinkRatio;*/
	const float halfW = renderer->GetSizeX() * 0.5f * scale.x;
	const float halfH = renderer->GetSizeY() * 0.5f * scale.y;

	const float left = pos.x - halfW;
	const float right = pos.x + halfW;
	const float bottom = pos.y - halfH;
	const float top = pos.y + halfH;

	// margin 비율 (예: 0.1f = 10% 여유)
	/*const float marginRatio = 0.01f;
	const float marginX = (view.maxX - view.minX) * marginRatio;
	const float marginY = (view.maxY - view.minY) * marginRatio;
	const float minX = view.minX - marginX;
	const float maxX = view.maxX + marginX;
	const float minY = view.minY - marginY;
	const float maxY = view.maxY + marginY;*/
	const float marginX = (view.maxX - view.minX);
	const float marginY = (view.maxY - view.minY);
	const float minX = view.minX;
	const float maxX = view.maxX;
	const float minY = view.minY;
	const float maxY = view.maxY;

	// 카메라 뷰 안에만 보이게 (여유 포함)
	return (right < minX || left > maxX ||
		top < minY || bottom > maxY);
}

bool D2DRenderManager::RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b)
{
	if (a.expired()) return false;
	if (b.expired()) return false;
	return a->m_layer < b->m_layer;
}

void D2DRenderManager::GetApplicationSize(int& width, int& height)
{
	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
}

void D2DRenderManager::CreateSwapChainAndD2DTarget()
{
	// 1. 현재 타겟 해제
	if (m_d2dDeviceContext)
	{
		m_d2dDeviceContext->SetTarget(nullptr);
	}
	m_d2dBitmapTarget.Reset();

	int width = 0, height = 0;
	GetApplicationSize(width, height);

	// 2. SwapChain 재설정
	if (m_dxgiSwapChain) {
		// ResizeBuffers: 넓이/높이가 0이면 현재 설정 유지, 여기선 실제 크기로 넘깁니다.
		m_dxgiSwapChain->ResizeBuffers(
			0,
			width,
			height,
			DXGI_FORMAT_UNKNOWN,
			0
		);
	}

	// 3. 백버퍼 IDXGISurface 얻기
	ComPtr<IDXGISurface> backBuffer;
	HRESULT hr = m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 4. D2D1BitmapProperties1 생성 
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// 5. DeviceContext에 바인딩할 ID2D1Bitmap1 생성
	hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, &m_d2dBitmapTarget);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 6. 컨텍스트에 SetTarget
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
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
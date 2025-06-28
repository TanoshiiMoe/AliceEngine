#pragma once

#include "pch.h"
#include "Component/RenderComponent.h"
#include "Component/TextRenderComponent.h"
#include "Component/BoxComponent.h"
#include "Object/gameObject.h"
#include <Component/AnimationComponent.h>

using namespace Define;
class D2DRenderManager : public Singleton<D2DRenderManager>
{
public:
	D2DRenderManager();
	~D2DRenderManager();

	// 렌더링 대기열
	std::vector<std::vector<std::weak_ptr<Component>>> m_renderers;

	inline void AddRenderer(std::weak_ptr<Component> renderer)
	{
		if (!renderer.expired())
		{
			auto sharedRenderer = renderer.lock();

			if (std::dynamic_pointer_cast<SpriteRenderer>(sharedRenderer))
			{
				m_renderers[static_cast<int>(ERenderLayer::SpriteComponent)].push_back(renderer);
			}
			else if (std::dynamic_pointer_cast<AnimationComponent>(sharedRenderer))
			{
				m_renderers[static_cast<int>(ERenderLayer::AnimationComponent)].push_back(renderer);
			}
			else if (std::dynamic_pointer_cast<BoxComponent>(sharedRenderer))
			{
				m_renderers[static_cast<int>(ERenderLayer::BoxComponent)].push_back(renderer);
			}
			else if (std::dynamic_pointer_cast<TextRenderComponent>(sharedRenderer))
			{
				m_renderers[static_cast<int>(ERenderLayer::TextRenderComponent)].push_back(renderer);
			}
		}
	}

	inline static ID2D1DeviceContext7* GetD2DDevice()
	{
		return GetInstance().m_d2dDeviceContext.Get();
	}

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void UnInitialize();
	void Update();
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
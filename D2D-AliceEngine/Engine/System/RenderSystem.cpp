#include "pch.h"
#include "RenderSystem.h"
#include "Component/TextRenderComponent.h"
#include <Component/SpriteRenderer.h>
#include <Component/VideoComponent.h>
#include <Component/BoxComponent.h>
#include <Component/Animator.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/UIComponent.h>
//#include <UI/UIImage.h>

RenderSystem::RenderSystem()
{
	m_renderers.clear();
}

RenderSystem::~RenderSystem()
{
	m_renderers.clear();
}

void RenderSystem::Regist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock()) // raw pointer 반환
	{
		m_renderers.push_back(renderer);
	}
}

void RenderSystem::UnRegist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock())
	{
		m_renderers.erase(std::remove_if(m_renderers.begin(), m_renderers.end(),
				[&](const WeakObjectPtr<RenderComponent>& r)
			{
			return r.handle == renderer.handle;
			}), m_renderers.end());
	}

}

void RenderSystem::UnRegistAll()
{
	m_renderers.clear();
}

void RenderSystem::Initialize()
{
	for (auto& renderer : m_renderers)
	{
		if (auto render = renderer.lock())
		{
			render->Initialize();
		}
	}
}

void RenderSystem::UnInitialize()
{
	for (auto& renderer : m_renderers)
	{
		if (auto render = renderer.lock())
		{
			render->Release();
		}
	}
	UnRegistAll();
	m_renderers.clear();
}

void RenderSystem::Render()
{
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	bool m_resizePending = D2DRenderManager::GetInstance().m_resizePending;
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
	if (m_resizePending)
	{
		D2DRenderManager::GetInstance().CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}
	m_d2dDeviceContext->SetTarget(D2DRenderManager::GetInstance().m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	ViewRect view = GetCameraView();

	sort(m_renderers.begin(), m_renderers.end(), &RenderSystem::RenderSortCompare);

	for (auto it = m_renderers.begin(); it != m_renderers.end(); )
	{
		if (it->expired())
		{
			it = m_renderers.erase(it);
			continue;
		}
		if (it->lock()->drawType == Define::EDrawType::WorldSpace && CheckCameraCulling(*it, view)) 
		{
			++it;
			continue;
		}

		if (auto renderer = it->lock())
		{
			renderer->Render();
		}
		++it;
	}

	HRESULT hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		D2DRenderManager::GetInstance().OutputError(hr);
	}

	D2DRenderManager::GetInstance().m_dxgiSwapChain->Present(1, 0);
}

ViewRect RenderSystem::GetCameraView()
{
	const float camX = SceneManager::GetCamera()->GetPositionX();
	const float camY = SceneManager::GetCamera()->GetPositionY();
	const float fov = SceneManager::GetCamera()->fieldOfView;
	FVector2 scale = SceneManager::GetCamera()->GetScale();
	if (scale.x == 0.0f) scale.x = 1.0f;
	if (scale.y == 0.0f) scale.y = 1.0f;
	int screenWidth = 0, screenHeight = 0;
	D2DRenderManager::GetInstance().GetApplicationSize(screenWidth, screenHeight);
	const float halfWidth = (screenWidth * 0.5f * fov) * scale.x;
	const float halfHeight = (screenHeight * 0.5f * fov) * scale.y;
	return ViewRect{ camX - halfWidth, camX + halfWidth, camY - halfHeight, camY + halfHeight };
}

// 컬링이 되는걸 확실하게 보려면 주석처리 된 부분을 사용
bool RenderSystem::CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view)
{
	auto* transform = renderer->GetTransform();
	const auto pos = transform ? transform->GetPosition() : D2D1_VECTOR_2F{ 0, 0 };
	const auto scale = transform ? transform->GetScale() : D2D1_VECTOR_2F{ 1, 1 };

	/*const float shrinkRatio = 0.5;
	const float halfW = renderer->GetSizeX() * 0.5f * scale.x * shrinkRatio;
	const float halfH = renderer->GetSizeY() * 0.5f * scale.y * shrinkRatio;*/
	const float halfW = renderer->GetBitmapSizeX() * 0.5f * scale.x;
	const float halfH = renderer->GetBitmapSizeY() * 0.5f * scale.y;

	const float left = pos.x - halfW;
	const float right = pos.x + halfW;
	const float bottom = pos.y - halfH;
	const float top = pos.y + halfH;

	// margin 비율 (예: 0.1f = 10% 여유)
	const float marginRatio = 0.05f;
	const float marginX = (view.maxX - view.minX) * marginRatio;
	const float marginY = (view.maxY - view.minY) * 1.2f;
	const float minX = view.minX - marginX;
	const float maxX = view.maxX + marginX;
	const float minY = view.minY - marginY;
	const float maxY = view.maxY + marginY;

	/*const float marginX = (view.maxX - view.minX);
	const float marginY = (view.maxY - view.minY);
	const float minX = view.minX;
	const float maxX = view.maxX;
	const float minY = view.minY;
	const float maxY = view.maxY;*/

	// 카메라 뷰 안에만 보이게 (여유 포함)
	return (right < minX || left > maxX ||
		top < minY || bottom > maxY);
}

bool RenderSystem::RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b)
{
	if (a.expired()) return false;
	if (b.expired()) return false;
	return a->m_layer < b->m_layer;
}
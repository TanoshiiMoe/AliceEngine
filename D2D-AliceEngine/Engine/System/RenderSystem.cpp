#include "pch.h"
#include "RenderSystem.h"
#include "Component/TextRenderComponent.h"
#include <Component/RenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/VideoComponent.h>
#include <Component/BoxComponent.h>
#include <Component/Animator.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/UIComponent.h>
#include <tuple>
//#include <UI/UIImage.h>

RenderSystem::RenderSystem()
{
	m_renderers.clear();
	m_renderQueue.clear();
}

RenderSystem::~RenderSystem()
{
	m_renderers.clear();
	m_renderQueue.clear();
}

void RenderSystem::Regist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock())
	{
		m_renderers.push_back(renderer);

		// 통합 렌더링 큐에도 추가
		int layer = ptr->m_layer;
		m_renderQueue.emplace_back(Define::ERenderType::D2D, renderer->GetHandle(), [renderer](){ renderer->Render(); }, renderer->drawType, renderer->m_layer);
	}
}

void RenderSystem::RegistSpine2D(ObjectHandle objectHandle, std::function<void()> f, int layer)
{
	// 기존 방식 유지 (하위 호환성)
	m_spineRenders.push_back({ objectHandle, f });

	// 통합 렌더링 큐에 추가
	//m_renderQueue.emplace_back(Define::ERenderType::D2D, objectHandle, [renderer]() { renderer->Render(); }, renderer->drawType, renderer->m_layer);
	//(objectHandle, f, layer);
}

void RenderSystem::UnRegist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock())
	{
		// 기존 렌더러 목록에서 제거
		m_renderers.erase(std::remove_if(m_renderers.begin(), m_renderers.end(),
			[&](const WeakObjectPtr<RenderComponent>& r)
			{
				return r.handle == renderer.handle;
			}), m_renderers.end());

		// 통합 렌더링 큐에서도 제거
		ObjectHandle targetHandle = ptr->GetHandle();
		m_renderQueue.erase(std::remove_if(m_renderQueue.begin(), m_renderQueue.end(),
			[&](const RenderItem& item)
			{
				return item.objectHandle == targetHandle && item.type == RenderItem::Type::D2D;
			}), m_renderQueue.end());
	}
}

void RenderSystem::UnRegistAll()
{
	m_renderers.clear();
	m_renderQueue.clear();
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
	m_renderQueue.clear();
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
	//sort(m_renderers.begin(), m_renderers.end(), &RenderSystem::RenderSortCompare);
	//RenderD2D();
	//RenderSpine2D();

	// 통합 렌더링 사용
	RenderUnified();

	HRESULT hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		D2DRenderManager::GetInstance().OutputError(hr);
	}

	D2DRenderManager::GetInstance().m_dxgiSwapChain->Present(1, 0);
}

void RenderSystem::RenderUnified()
{
	// 만료된 아이템 제거
	m_renderQueue.erase(std::remove_if(m_renderQueue.begin(), m_renderQueue.end(),
		[](const RenderItem& item) { return !item.IsValid(); }), m_renderQueue.end());

	// 레이어 순서로 정렬
	std::sort(m_renderQueue.begin(), m_renderQueue.end(), RenderItemSortCompare);

	ViewRect view = GetCameraView();

	// 통합 렌더링 루프
	for (const auto& item : m_renderQueue)
	{
		if (!item.IsValid()) continue;

		if (item.type == RenderItem::Type::D2D)
		{
			// D2D 렌더링
			if (auto renderer = item.d2dRenderer.lock())
			{
				// 카메라 컬링 체크 (WorldSpace인 경우에만)
				if (renderer->drawType == Define::EDrawType::WorldSpace &&
					CheckCameraCulling(item.d2dRenderer, view))
				{
					continue;
				}

				renderer->Render();
			}
		}
		else if (item.type == RenderItem::Type::Spine2D)
		{
			// Spine2D 렌더링
			if (ObjectHandler::GetInstance().IsValid(item.objectHandle))
			{
				item.spine2dRenderFunc();
			}
		}
	}
}

void RenderSystem::RenderD2D()
{
	ViewRect view = GetCameraView();
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
}

void RenderSystem::RenderSpine2D()
{
	for (auto it = m_spineRenders.begin(); it != m_spineRenders.end(); )
	{
		if (!ObjectHandler::GetInstance().IsValid(it->first))
		{
			it = m_spineRenders.erase(it);
			continue;
		}
		it->second();
		++it;
	}
}

ViewRect RenderSystem::GetCameraView()
{
	const float camX = SceneManager::GetCamera()->GetPositionX();
	const float camY = SceneManager::GetCamera()->GetPositionY();
	const float fov = SceneManager::GetCamera()->fieldOfView;
	FVector2 scale = SceneManager::GetCamera()->GetScale();
	if (scale.x == 0.0f) scale.x = 1.0f;
	if (scale.y == 0.0f) scale.y = 1.0f;
	FVector2 screen = D2DRenderManager::GetInstance().GetApplicationSize();
	const float halfWidth = (screen.x * 0.5f * fov) * scale.x;
	const float halfHeight = (screen.y * 0.5f * fov) * scale.y;
	return ViewRect{ camX - halfWidth, camX + halfWidth, camY - halfHeight, camY + halfHeight };
}

// 컬링이 되는걸 확실하게 보려면 주석처리 된 부분을 사용
bool RenderSystem::CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view)
{
	auto* transform = renderer->GetOwnerTransform();
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

bool RenderSystem::RenderItemSortCompare(const RenderItem& a, const RenderItem& b)
{
	return a.layer < b.layer;
}
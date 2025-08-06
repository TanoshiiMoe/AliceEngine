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
#include <Define/Define.h>
//#include <UI/UIImage.h>

RenderItem::RenderItem()
	: type(Define::ERenderType::D2D),
	layer(0),
	objectHandle(),
	drawType(Define::EDrawType::WorldSpace),
	D2DObject(nullptr),
	RenderFunc(nullptr)
{
}

RenderItem::RenderItem(ObjectHandle handle, RenderComponent* object, std::function<void()> func, Define::EDrawType _drawType, int* renderLayer)
	: type(Define::ERenderType::D2D), objectHandle(handle), D2DObject(object), RenderFunc(func), drawType(_drawType), layer(renderLayer)
{
	if (!ObjectHandler::GetInstance().IsValid(handle)) {
		// ��ȿ���� ������ ���� ������ �� IsValid()���� �ɷ����Ƿ� �� �ڸ����� ���� Ż������ �ʾƵ� �˴ϴ�.
	}
}

RenderItem::RenderItem(Define::ERenderType _type, ObjectHandle handle, std::function<void()> func, Define::EDrawType _drawType, int* renderLayer)
	: type(_type), objectHandle(handle), RenderFunc(func), drawType(_drawType), layer(renderLayer)
{
}

bool RenderItem::IsValid() const
{
	return ObjectHandler::GetInstance().IsValid(objectHandle);
}

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

		// ���� ������ ť���� �߰�
		m_renderQueue.emplace_back(
			renderer->GetHandle(),
			renderer.Get(),
			[renderer](){ renderer->Render(); },
			renderer->drawType,
			&renderer->m_layer
		);
	}
}

void RenderSystem::RegistSpine2D(ObjectHandle objectHandle, std::function<void()> f, Define::EDrawType _drawType, int* _layer)
{
	// ���� ��� ���� (���� ȣȯ��)
	m_spineRenders.push_back({ objectHandle, f });
	// ���� ������ ť�� �߰�
	m_renderQueue.emplace_back(
		Define::ERenderType::Spine2D,
		objectHandle,
		f,
		_drawType,
		_layer
	);
}

void RenderSystem::UnRegist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock())
	{
		// ���� ������ ��Ͽ��� ����
		m_renderers.erase(std::remove_if(m_renderers.begin(), m_renderers.end(),
			[&](const WeakObjectPtr<RenderComponent>& r)
			{
				return r.handle == renderer.handle;
			}), m_renderers.end());

		// ���� ������ ť������ ����
		ObjectHandle targetHandle = ptr->GetHandle();
		m_renderQueue.erase(std::remove_if(m_renderQueue.begin(), m_renderQueue.end(),
			[&](const RenderItem& item)
			{
				return item.objectHandle == targetHandle && item.type == Define::ERenderType::D2D;
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
	if (!m_d2dDeviceContext.Get()) return;
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

	// ���� ������ ���
	RenderUnified();
	DebugCamera();
	

	HRESULT hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		D2DRenderManager::GetInstance().OutputError(hr);
	}

	D2DRenderManager::GetInstance().m_dxgiSwapChain->Present(1, 0);
}

void RenderSystem::RenderUnified()
{
	// ����� ������ ����
	m_renderQueue.erase(std::remove_if(m_renderQueue.begin(), m_renderQueue.end(),
		[](const RenderItem& item) { return !item.IsValid(); }), m_renderQueue.end());

	// ���̾� ������ ����
	std::sort(m_renderQueue.begin(), m_renderQueue.end(), RenderItemSortCompare);

	ViewRect view = GetCameraView();

	// ���� ������ ����
	for (const auto& item : m_renderQueue)
	{
		if (!item.IsValid()) continue;

		if (item.type == Define::ERenderType::D2D)
		{
			// D2D ������
			if (ObjectHandler::GetInstance().IsValid(item.objectHandle))
			{
				// ī�޶� �ø� üũ (WorldSpace�� ��쿡��)
				switch (item.drawType)
				{
				case Define::EDrawType::WorldSpace:
					if (!CheckCameraCulling(item.D2DObject, view))
					{
						item.RenderFunc();
					}
					break;
				case Define::EDrawType::ScreenSpace:
					// ScreenSpace�� �ø� üũ�� ���� ����
					item.RenderFunc();
					break;
				default:
					break;
				}
				//renderer->Render();
			}
		}
		else if (item.type == Define::ERenderType::Spine2D)
		{
			// Spine2D ������
			if (ObjectHandler::GetInstance().IsValid(item.objectHandle))
			{
				item.RenderFunc();
			}
		}
	}
}

void RenderSystem::DebugCamera()
{
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	if (!m_d2dDeviceContext.Get()) return;
	if (Camera* camera = SceneManager::GetCamera())
	{
		if (camera->bDebug)
		{
			D2D1::Matrix3x2F flipY = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
			D2D1::Matrix3x2F screen = D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
			D2D1::Matrix3x2F cameraInv = camera->relativeTransform.m_worldTransform.ToMatrix();
			cameraInv.Invert();
			m_d2dDeviceContext->SetTransform(cameraInv * flipY * screen);

			FVector2 pos = SceneManager::GetCamera()->GetRelativePosition();
			D2DRenderManager::GetInstance().DrawDebugBox(pos.x - 10, pos.y - 10, pos.x + 10, pos.y + 10, 0, 0, 255, 255);

			m_d2dDeviceContext->SetTransform(cameraInv * screen);
			D2DRenderManager::GetInstance().DrawDebugText(L"(" + std::to_wstring(pos.x) + L" " + std::to_wstring(pos.y) + L")", pos.x, pos.y, 24, D2D1::ColorF(0, 0, 255, 1));
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
	//FVector2 scale = SceneManager::GetCamera()->GetScale();
	//if (scale.x == 0.0f) scale.x = 1.0f;
	//if (scale.y == 0.0f) scale.y = 1.0f;
	FVector2 screen = D2DRenderManager::GetInstance().GetApplicationSize();
	//const float halfWidth = (screen.x * 0.5f * fov) * scale.x;
	//const float halfHeight = (screen.y * 0.5f * fov) * scale.y;
	const float halfWidth = (screen.x * 0.5f * fov);
	const float halfHeight = (screen.y * 0.5f * fov);
	return ViewRect{ camX - halfWidth, camX + halfWidth, camY - halfHeight, camY + halfHeight };
}

// �ø��� �Ǵ°� Ȯ���ϰ� ������ �ּ�ó�� �� �κ��� ���
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

	// margin ���� (��: 0.1f = 10% ����)
	const float marginRatio = 0.05f;
	const float marginX = (view.maxX - view.minX) * marginRatio;
	const float marginY = (view.maxY - view.minY) * 1.2f;
	const float minX = view.minX - marginX;
	const float maxX = view.maxX + marginX;
	const float minY = view.minY - marginY;
	const float maxY = view.maxY + marginY;

	// ī�޶� �� �ȿ��� ���̰� (���� ����)
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
	return *(a.layer) < *(b.layer);
}
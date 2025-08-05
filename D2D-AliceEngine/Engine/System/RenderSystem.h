#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"
#include <Define/Define.h>
#include <tuple>

/* 
*  @briefs : �������� ����ϴ� �ý����Դϴ�
*/
class RenderComponent;
struct ViewRect { float minX, maxX, minY, maxY; };

// ������ �������� ���� �����ϱ� ���� ����ü
struct RenderItem
{
	Define::ERenderType type;
	int layer;
	ObjectHandle objectHandle;
	Define::EDrawType drawType = Define::EDrawType::WorldSpace;

	// Spine2D�� ������
	std::function<void()> spine2dRenderFunc;

	// Spine2D �������� ������
	RenderItem(Define::ERenderType _type, ObjectHandle handle, std::function<void()> func, Define::EDrawType& _drawType, int renderLayer)
		: type(_type), layer(renderLayer), objectHandle(handle), drawType(_drawType), spine2dRenderFunc(func)
	{
	}

	bool IsValid() const
	{
		return ObjectHandler::GetInstance().IsValid(objectHandle);
	}
};

class RenderSystem : public Singleton<RenderSystem>
{
public:
	RenderSystem();
	~RenderSystem();

public:
	void Regist(WeakObjectPtr<RenderComponent>&& renderer);
	void UnRegist(WeakObjectPtr<RenderComponent>&& renderer);
	void UnRegistAll();

	void Initialize();
	void UnInitialize();

	// ������ ��⿭
	std::vector<WeakObjectPtr<RenderComponent>> m_renderers;

	// ���� ������ ť
	std::vector<RenderItem> m_renderQueue;

	void Render();
	void RenderUnified(); // ���ο� ���� ������ �Լ�
	void RenderD2D();
	void RenderSpine2D();

	ViewRect GetCameraView();
	bool CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view);
	static bool RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b);
	static bool RenderItemSortCompare(const RenderItem& a, const RenderItem& b);

	void RegistSpine2D(ObjectHandle objectHandle, std::function<void()> f, int layer = 0);
	std::vector<std::pair<ObjectHandle, std::function<void()>>> m_spineRenders;
};
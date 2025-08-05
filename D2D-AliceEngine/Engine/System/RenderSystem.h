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
	int* layer{nullptr};
	ObjectHandle objectHandle;
	Define::EDrawType drawType = Define::EDrawType::WorldSpace;
	RenderComponent* D2DObject = nullptr;

	// Spine2D�� ������
	std::function<void()> RenderFunc;
	RenderItem();
	// D2D �������� ������
	explicit RenderItem(ObjectHandle handle, RenderComponent* object, std::function<void()> func, Define::EDrawType _drawType, int* renderLayer);
	// Spine2D �������� ������
	explicit RenderItem(Define::ERenderType _type, ObjectHandle handle, std::function<void()> func, Define::EDrawType _drawType, int* renderLayer);

	bool IsValid() const;
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

	void RegistSpine2D(ObjectHandle objectHandle, std::function<void()> f, Define::EDrawType _drawType = Define::EDrawType::ScreenSpace, int* _layer = 0);
	std::vector<std::pair<ObjectHandle, std::function<void()>>> m_spineRenders;
};
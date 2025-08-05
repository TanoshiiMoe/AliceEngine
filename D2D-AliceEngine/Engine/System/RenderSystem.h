#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"
#include <Define/Define.h>
#include <tuple>

/* 
*  @briefs : 렌더링을 담당하는 시스템입니다
*/
class RenderComponent;
struct ViewRect { float minX, maxX, minY, maxY; };

// 렌더링 아이템을 통합 관리하기 위한 구조체
struct RenderItem
{
	Define::ERenderType type;
	int layer;
	ObjectHandle objectHandle;
	Define::EDrawType drawType = Define::EDrawType::WorldSpace;

	// Spine2D용 데이터
	std::function<void()> spine2dRenderFunc;

	// Spine2D 렌더러용 생성자
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

	// 렌더링 대기열
	std::vector<WeakObjectPtr<RenderComponent>> m_renderers;

	// 통합 렌더링 큐
	std::vector<RenderItem> m_renderQueue;

	void Render();
	void RenderUnified(); // 새로운 통합 렌더링 함수
	void RenderD2D();
	void RenderSpine2D();

	ViewRect GetCameraView();
	bool CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view);
	static bool RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b);
	static bool RenderItemSortCompare(const RenderItem& a, const RenderItem& b);

	void RegistSpine2D(ObjectHandle objectHandle, std::function<void()> f, int layer = 0);
	std::vector<std::pair<ObjectHandle, std::function<void()>>> m_spineRenders;
};
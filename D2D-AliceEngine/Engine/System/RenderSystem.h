#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"

/* 
*  @briefs : �������� ����ϴ� �ý����Դϴ�
*/

class RenderComponent;
struct ViewRect { float minX, maxX, minY, maxY; };
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
	std::vector<std::vector<WeakObjectPtr<RenderComponent>>> m_renderers;

	void Render();
	ViewRect GetCameraView();
	bool CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view);
	static bool RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b);
};
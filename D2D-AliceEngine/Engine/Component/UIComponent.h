#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Math/Transform.h>
#include "InputComponent.h"
#include <Define/Define.h>

/*
*	UI 컴포넌트입니다.
*	텍스트는 UIText(카메라에 고정), 고정 없는 Text로 분리
*	마우스 클릭 이벤트 처리는 버튼에서만 처리하면 됩니다.
*	뭐 이미지도 클릭할 수 있게 한다면 사용하셔도 되긴 함 ㅇㅅㅇ...
*	추가적으로 이미지가 애니메이션도 있을 수 있으니 업데이트도 가져왔어요
*/

using namespace Define;
class gameObject;
class UIComponent : public RenderComponent
{
public:
	UIComponent();
	~UIComponent();

	virtual void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();

	Transform m_transform;

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}

	virtual void SetPosition(const FVector2& pos);
	virtual void SetPivot(float _x, float _y);
	virtual void SetAnchor(EUIScreenAnchor anchor, const FVector2& offset = FVector2(0, 0));
	virtual void SetAnchor(EUIScreenAnchor anchor, const float& offsetX, const float& offsetY);

	EUIScreenAnchor GetAnchor() { return m_anchor; }
	EUIScreenAnchor m_anchor = EUIScreenAnchor::TopLeft;

private:
	UIComponent* m_parents = nullptr;
	std::vector<UIComponent*> m_child;

private:
	struct ClickSlot 
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ClickSlot> slots;
};
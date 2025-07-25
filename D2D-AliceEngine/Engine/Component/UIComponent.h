#pragma once
#include <Component/RenderComponent.h>
#include <Manager/UIManager.h>

/*
*	UI 컴포넌트입니다.
*	텍스트는 UIText(카메라에 고정), 고정 없는 Text로 분리
*	마우스 클릭 이벤트 처리는 버튼에서만 처리하면 됩니다.
*	뭐 이미지도 클릭할 수 있게 한다면 사용하셔도 되긴 함 ㅇㅅㅇ...
*	추가적으로 이미지가 애니메이션도 있을 수 있으니 업데이트도 가져왔어요
*/

class UIComponent : public RenderComponent
{
public:
	UIComponent();
	~UIComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	void Render() override;

	virtual float GetSizeX() override;
	virtual float GetSizeY() override;

	FVector2 GetSize();

	virtual void OnMouseEvent(const POINT& mousePos, bool isClick) {}

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};


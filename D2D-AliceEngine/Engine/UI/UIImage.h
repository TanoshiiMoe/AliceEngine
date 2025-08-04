#pragma once
#include <Component/UIComponent.h>
#include <Animation/TextureLoader.h>
#include <Define/Define.h>
#include <Math/Transform.h>

using namespace Microsoft::WRL;
using namespace Define;
struct ID2D1Bitmap1;
class UIImage : public UIComponent
{
public:
	UIImage();
	virtual ~UIImage();

	void LoadData(const std::wstring& path);
	//void SetScale(const FVector2& _scale) { scale = _scale; }
	//void SetScale(const float& _x, const float& _y) { scale = FVector2(_x, _y); }
	//void SetScale(const float& _x) { scale = FVector2(_x, _x); }

	void SetPosition(const FVector2& pos) override {}
	void SetPivot(float _x, float _y);
	void SetAnchor(EUIScreenAnchor anchor, const FVector2& offset = FVector2(0, 0)) override {}
	void SetAnchor(EUIScreenAnchor anchor, const float& offsetX, const float& offsetY) override {}

	EUIScreenAnchor GetAnchor() { return m_anchor; }
	EUIScreenAnchor m_anchor = EUIScreenAnchor::TopLeft;

private:
	/*	보류
	float m_opacity = 1.0f;      // 예: 페이드 효과
	FVector2 m_startPos;         // 예: 이동 애니메이션
	FVector2 m_targetPos;
	float m_moveTime = 0.0f;     // 애니메이션 시간 경과
	float m_totalTime = 1.0f;    // 총 애니메이션 시간
	*/
};
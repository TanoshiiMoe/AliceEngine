#pragma once
#include <Component/UIComponent.h>
#include <Animation/TextureLoader.h>

using namespace Microsoft::WRL;

struct ID2D1Bitmap1;
class UIImage : public UIComponent
{
public:
	UIImage();
	virtual ~UIImage();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

private:
	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	/*	보류
	float m_opacity = 1.0f;      // 예: 페이드 효과
	FVector2 m_startPos;         // 예: 이동 애니메이션
	FVector2 m_targetPos;
	float m_moveTime = 0.0f;     // 애니메이션 시간 경과
	float m_totalTime = 1.0f;    // 총 애니메이션 시간
	*/
};


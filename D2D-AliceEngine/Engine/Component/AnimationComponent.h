#pragma once
#include "pch.h"
#include <Math/Transform.h>
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"

/*
*	@briefs : 애니메이션 컴포넌트입니다. 
*	@details : 
*	mp4 파일을 변환하여 이미지로 변환한 후에 실행합니다.
*	현재 프레임 앞 뒤 cacheSize 만큼을 할당, 해제하며 그립니다.
*/

using namespace Microsoft::WRL;
class AnimationComponent : public RenderComponent
{
public:
	AnimationComponent() {}
	~AnimationComponent() 
	{
		files.clear();
		m_bitmaps.clear();
	}

	void Initialize() override;
	void Update() override;
	void UpdateFrames();
	void ReleaseFrames();
	void LoadData(const std::wstring& path, const int& fps);
	void LoadFrame(size_t frameIndex);
	void Release() override;
	void Render() override;

	FVector2 GetSize()
	{
		if (m_bitmaps.empty() == false)
		{
			ComPtr<ID2D1Bitmap1> bitmapStrong;
			if (m_bitmaps[0].lock())
			{
				D2D1_SIZE_U bmpSize = m_bitmaps[0].lock()->GetPixelSize();
				return FVector2(bmpSize.width, bmpSize.height);
			}
			return FVector2(0,0);
		}
		return FVector2();
	}
	std::vector<std::wstring> files;
	std::vector<std::weak_ptr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage 컴포넌트
	const size_t cacheSize = 2; // 캐시할 프레임 수

	void Play() { bPlay = true; }
	void Stop() { bPlay = false; }

	size_t m_curClip = 0;
	size_t m_maxClipSize = 0;
	bool bPlay = false;
	float m_fFPSTime = 1.0f / 18.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
};


#pragma once
#include <spine/spine.h>
#include <spine/extension.h>
#include <d2d1_1.h>
#include <string>

//SpineExtension* getDefaultExtension() {
//	return new DefaultSpineExtension(); // 표준 new/delete 사용
//}

namespace spine
{
	class SkeletonDrawable
	{
	public:
		SkeletonDrawable(ID2D1Bitmap1* _bitmap, const std::wstring& path);
		SkeletonDrawable(SkeletonData* skeletonData, AnimationStateData* stateData = nullptr);
		~SkeletonDrawable();

		void Update(float deltaTime);
		void Draw(ID2D1DeviceContext* context);

		void SetPremultipliedAlpha(bool enabled) { usePremultipliedAlpha = enabled; }
		bool GetPremultipliedAlpha() const { return usePremultipliedAlpha; }

	private:
		Skeleton* skeleton;
		AnimationState* state;
		AnimationStateData* stateData;
		bool ownsStateData;

		float timeScale = 1.0f;
		bool usePremultipliedAlpha = false;

		Vector<float> worldVertices;
		Vector<float> tempUvs;
		Vector<Color> tempColors;
		Vector<unsigned short> quadIndices;
		SkeletonClipping clipper;

		//ComPtr<ID2D1SolidColorBrush> debugBrush;

		void InitializeBrush(ID2D1DeviceContext* context);
	};
}
#pragma once

#include <spine/spine.h>
#include <spine/extension.h>
#include <d2d1_1.h>
#include <wrl/client.h>
#include <Vector>
#include <iostream>

spine::SpineExtension* spine::getDefaultExtension() {
	return new spine::DefaultSpineExtension(); // 표준 malloc/free 사용
}

namespace spine {

    using Microsoft::WRL::ComPtr;

    class SkeletonDrawable_D2D 
    {
    public:
        SkeletonDrawable_D2D(ID2D1Bitmap1* _bitmap, const std::wstring& path);
        SkeletonDrawable_D2D(SkeletonData* skeletonData, AnimationStateData* stateData = nullptr);
        ~SkeletonDrawable_D2D();

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

        ComPtr<ID2D1SolidColorBrush> debugBrush;

        void InitializeBrush(ID2D1DeviceContext* context);
    };
} // namespace spine






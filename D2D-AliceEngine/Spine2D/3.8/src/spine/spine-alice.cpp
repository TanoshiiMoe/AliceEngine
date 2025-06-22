#include "pch.h"
#include <spine/spine-alice.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/ClippingAttachment.h>

#include <cmath>

namespace spine {

    SkeletonDrawable_D2D::SkeletonDrawable_D2D(SkeletonData* skeletonData, AnimationStateData* stateData)
        : skeleton(new Skeleton(skeletonData)), stateData(stateData), ownsStateData(false) {

        if (!stateData) {
            this->stateData = new AnimationStateData(skeletonData);
            ownsStateData = true;
        }

        state = new AnimationState(this->stateData);

        worldVertices.setSize(1000, 0);  // 대충 여유롭게 잡아둠
        tempUvs.ensureCapacity(1000);
        tempColors.ensureCapacity(1000);
        quadIndices.ensureCapacity(1000);
    }

    SkeletonDrawable_D2D::~SkeletonDrawable_D2D() {
        delete skeleton;
        delete state;
        if (ownsStateData) delete stateData;
    }

    void SkeletonDrawable_D2D::Update(float deltaTime) {
        state->update(deltaTime * timeScale);
        state->apply(*skeleton);
        skeleton->updateWorldTransform();
    }

    void SkeletonDrawable_D2D::Draw(ID2D1DeviceContext* context) {
        if (!context) return;

        if (!debugBrush) {
            InitializeBrush(context);
        }

        context->SetTransform(D2D1::Matrix3x2F::Identity());

        Vector<Slot*>& drawOrder = skeleton->getDrawOrder();
        for (int i = 0, n = drawOrder.size(); i < n; ++i) {
            Slot* slot = drawOrder[i];
            Attachment* attachment = slot->getAttachment();
            if (!attachment) continue;

            if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
                RegionAttachment* region = (RegionAttachment*)attachment;
                worldVertices.setSize(8, 0);
                region->computeWorldVertices(slot->getBone(), worldVertices, 0, 2);

                for (int j = 0; j < 4; ++j) {
                    D2D1_POINT_2F pt = {
                        worldVertices[j * 2],
                        worldVertices[j * 2 + 1]
                    };

                    D2D1_ELLIPSE ellipse = D2D1::Ellipse(pt, 2.0f, 2.0f);
                    context->FillEllipse(&ellipse, debugBrush.Get());
                }

                // Draw connecting lines (outline)
                for (int j = 0; j < 4; ++j) {
                    D2D1_POINT_2F p1 = { worldVertices[j * 2], worldVertices[j * 2 + 1] };
                    D2D1_POINT_2F p2 = { worldVertices[((j + 1) % 4) * 2], worldVertices[((j + 1) % 4) * 2 + 1] };
                    context->DrawLine(p1, p2, debugBrush.Get(), 1.0f);
                }
            }
        }
    }

    void SkeletonDrawable_D2D::InitializeBrush(ID2D1DeviceContext* context) {
        D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Red);
        context->CreateSolidColorBrush(color, &debugBrush);
    }

} // namespace spine

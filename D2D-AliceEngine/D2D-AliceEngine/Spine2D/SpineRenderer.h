#pragma once

#include <wrl.h>  // ComPtr ����� ���� ���

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")

using namespace Microsoft::WRL;
using Microsoft::WRL::ComPtr;

#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>
#include <spine/SkeletonData.h>
#include <spine/AtlasAttachmentLoader.h>
#include <spine/SkeletonJson.h>
#include <spine/TextureLoader.h>
#include <wrl/client.h>
#include <d2d1_3.h>
#include <string>
#include <vector>
#include <map>

// Direct2D�� TextureLoader ����
class Direct2DTextureLoader : public spine::TextureLoader {
public:
    Direct2DTextureLoader(ID2D1RenderTarget* renderTarget);
    virtual ~Direct2DTextureLoader();
    void load(spine::AtlasPage& page, const spine::String& path) override;
    void unload(void* texture) override;
private:
    ID2D1RenderTarget* m_renderTarget;
    std::map<std::string, std::shared_ptr<ID2D1Bitmap>> m_bitmapMap;
};

class SpineRenderer
{
public:
    SpineRenderer();
    ~SpineRenderer();

    // �ʱ�ȭ
    void RegistSystem(WeakObjectPtr<UObject> object);
    void Initialize();
    void LoadTextureLoader();
    void Shutdown();

    // �ִϸ��̼� ����
    void SetAnimation(const std::string& animationName);
    void UpdateAnimation(float deltaTime);
    void SetAnimationTime(float time);
    float GetAnimationTime() const { return m_animationTime; }
    void SetNextAnimation();
    void SetPreviousAnimation();

    void HandleKeyInput(int keyCode);
    void Clear(const D2D1_COLOR_F& color = { 0.1f, 0.1f, 0.1f, 1.0f });
    D2D1_SIZE_F GetRenderTargetSize() const;

    // ������
    void Render();

    void LoadSpine(const std::wstring& atlasPath, const std::wstring& jsonPath);

    void ReleaseSpine();

    void SetDrawType(Define::EDrawType _type) { m_drawType = _type; }
    Define::EDrawType GetDrawType() { return m_drawType; }

    void SetLayer(int _layer) { m_layer = _layer; }
    int GetLayer () { return m_layer; }
    int m_layer = 1000;

private:
    D2D1::Matrix3x2F m_UnityScreen;

    // Spine-cpp ��ü
    std::unique_ptr<Direct2DTextureLoader> m_textureLoader;

    std::unique_ptr<spine::Atlas> m_atlas;
    std::unique_ptr<spine::SkeletonData> m_skeletonData;
    std::unique_ptr<spine::Skeleton> m_skeleton;
    std::unique_ptr<spine::AnimationStateData> m_stateData;
    std::unique_ptr<spine::AnimationState> m_state;

    // �ִϸ��̼� ��� �� ����
    std::vector<std::string> m_animationList;
    int m_currentAnimationIndex = 0;
    float m_animationTime = 0.0f;
    std::string m_currentAnimation = "idle";
    float m_currentAnimationTime = 0.0f;

    // ������ �ڵ� �� ũ��
    HWND m_hwnd;
    int m_clientWidth = 0;
    int m_clientHeight = 0;
    bool m_initialized = false;
    D2D1_VECTOR_2F m_CharacterPosition = D2D1::Vector2F(0.0f, 0.0f);
    D2D1_VECTOR_2F m_CameraPosition = D2D1::Vector2F(0.0f, 300.0f);

    Define::EDrawType m_drawType = Define::EDrawType::ScreenSpace;

    // ���� �Լ���(Direct2D/3D �ʱ�ȭ ��)
    void ReleaseResources();
    void ReleaseDirect2D();
};
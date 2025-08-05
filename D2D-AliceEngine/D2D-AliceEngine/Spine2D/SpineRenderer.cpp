#include "pch.h"
#include "SpineRenderer.h"
#include <Extension/json.hpp>// nlohmann::json ���
#include <wincodec.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <spine/Atlas.h>
#include <spine/SkeletonJson.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>
#include <spine/Skeleton.h>
#include <spine/AtlasAttachmentLoader.h>
#include <spine/TextureLoader.h>
#include <spine/Slot.h>
#include <spine/RegionAttachment.h>
#include <spine/SkeletonClipping.h>
#include <spine/Bone.h>
#include <spine/Animation.h>
#include <spine/BoneData.h>

#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib,"dwrite.lib")

#include <Core/Singleton.h>
#include <Manager/D2DRenderManager.h>


// Direct2DTextureLoader ����
Direct2DTextureLoader::Direct2DTextureLoader(ID2D1RenderTarget* renderTarget)
	: m_renderTarget(renderTarget) {
}
Direct2DTextureLoader::~Direct2DTextureLoader() {}
void Direct2DTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
	std::wstring wPath(path.buffer(), path.buffer() + path.length());
	Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	if (FAILED(hr))
		return;

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	hr = wicFactory->CreateDecoderFromFilename(wPath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
		return;

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
		return;

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr))
		return;
	hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
		return;

	Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
	hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), bitmap.GetAddressOf());
	if (FAILED(hr))
		return;

	page.texture = bitmap.Get();
	m_bitmapMap[path.buffer()] = bitmap;
	page.width = static_cast<int>(bitmap->GetSize().width);
	page.height = static_cast<int>(bitmap->GetSize().height);
}
void Direct2DTextureLoader::unload(void* texture)
{
	// ComPr ����
	m_bitmapMap.clear();
}


SpineRenderer::SpineRenderer() : m_textureLoader(nullptr), m_atlas(nullptr), m_skeletonData(nullptr), m_skeleton(nullptr), m_stateData(nullptr), m_state(nullptr) {}
SpineRenderer::~SpineRenderer() { Shutdown(); }

bool SpineRenderer::Initialize(HWND hwnd, int width, int height) {
	m_hwnd = hwnd;

	m_UnityScreen = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
		D2D1::Matrix3x2F::Translation((float)width / 2, (float)height / 2);

	m_clientWidth = width;
	m_clientHeight = height;

	m_textureLoader = std::make_unique<Direct2DTextureLoader>(D2DRenderManager::GetInstance().m_d2dDeviceContext.Get());

	// spine-cpp ��� Spine ������ �ε�
	if (!LoadSpine("../Resource/Spine2D/Monster_1.atlas", "../Resource/Spine2D/Monster_1.json")) {
		std::cout << "Spine2D(spine-cpp) initialization failed" << std::endl;
		return false;
	}
	m_initialized = true;
	return true;
}

void SpineRenderer::Shutdown()
{
	ReleaseSpine();
	ReleaseDirect2D();
}

bool SpineRenderer::InitializeD3D11() {
	// Direct3D 11 ����̽� ����
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&m_device,
		&featureLevel,
		&m_deviceContext
	);

	if (FAILED(hr)) {
		return false;
	}

	// DXGI ���丮 ����
	ComPtr<IDXGIFactory> dxgiFactory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgiFactory);
	if (FAILED(hr)) {
		return false;
	}

	// ����ü�� ����
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = m_clientWidth;
	swapChainDesc.BufferDesc.Height = m_clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(m_device.Get(), &swapChainDesc, &m_swapChain);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool SpineRenderer::InitializeD2D1() {
	// Direct2D 1 ���丮 ����
	D2D1_FACTORY_OPTIONS options = {};
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		options,
		m_factory.GetAddressOf()
	);

	if (FAILED(hr)) {
		return false;
	}

	// Direct2D ���� Ÿ�� ����
	ComPtr<IDXGISurface> dxgiSurface;
	hr = m_swapChain->GetBuffer(0, __uuidof(IDXGISurface), &dxgiSurface);
	if (FAILED(hr)) {
		return false;
	}

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = {};
	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED
	);

	//hr = m_factory->CreateDxgiSurfaceRenderTarget(
	//	dxgiSurface.Get(),
	//	renderTargetProperties,
	//	&D2DRenderManager::GetInstance().m_d2dDeviceContext
	//);

	if (FAILED(hr)) {
		return false;
	}

	// �귯�� ����
	hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_brush
	);

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool SpineRenderer::InitializeDWrite() {
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_dwriteFactory.GetAddressOf())
	);
	if (FAILED(hr)) {
		std::cout << "DWriteCreateFactory failed" << std::endl;
		return false;
	}
	hr = m_dwriteFactory->CreateTextFormat(
		L"���� ���", // �ѱ� ��Ʈ
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"ko-kr",
		&m_textFormat
	);
	if (FAILED(hr)) {
		std::cout << "CreateTextFormat failed" << std::endl;
		return false;
	}
	return true;
}


void SpineRenderer::ReleaseResources() {

}

void SpineRenderer::BeginRender() {
	if (!m_initialized) return;

	// ���� Ÿ�� ����
	D2DRenderManager::GetInstance().m_d2dDeviceContext->BeginDraw();
}

void SpineRenderer::EndRender() {
	if (!m_initialized) return;

	// ���� Ÿ�� ����
	HRESULT hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->EndDraw();
	if (SUCCEEDED(hr)) {
		// ����ü�� ������Ʈ
		m_swapChain->Present(1, 0);
	}
}


void SpineRenderer::SetAnimation(const std::string& animationName) {
	if (m_state && m_skeletonData) {
		m_state->setAnimation(0, animationName.c_str(), true);
		m_currentAnimation = animationName;
		m_currentAnimationTime = 0.0f;
		m_animationTime = 0.0f;
		for (size_t i = 0; i < m_animationList.size(); ++i) {
			if (m_animationList[i] == animationName) {
				m_currentAnimationIndex = static_cast<int>(i);
				break;
			}
		}
		std::cout << "Animation changed: " << animationName << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
	}
}

void SpineRenderer::UpdateAnimation(float deltaTime) {
	if (m_state && m_skeleton) {
		m_state->update(deltaTime);
		m_state->apply(*m_skeleton.get());
		m_skeleton->updateWorldTransform(spine::Physics::Physics_Pose);
		m_animationTime += deltaTime;
		m_currentAnimationTime += deltaTime;
	}
}

void SpineRenderer::SetAnimationTime(float time) {
	m_animationTime = time;
}


D2D1_SIZE_F SpineRenderer::GetRenderTargetSize() const {
	if (D2DRenderManager::GetInstance().m_d2dDeviceContext) {
		return D2DRenderManager::GetInstance().m_d2dDeviceContext->GetSize();
	}
	return D2D1::SizeF(static_cast<float>(m_clientWidth), static_cast<float>(m_clientHeight)); // �⺻ ũ��
}

void SpineRenderer::Clear(const D2D1_COLOR_F& color) {
	if (!m_initialized) return;

	D2DRenderManager::GetInstance().m_d2dDeviceContext->Clear(color);
}



// --- ���Ժ� �̹��� ������ ---
void SpineRenderer::Render()
{
	if (!m_skeleton || !m_atlas || !D2DRenderManager::GetInstance().m_d2dDeviceContext)
		return;

	D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);

	// ĳ������ ���� ��ġ
	D2D1::Matrix3x2F worldTransform = D2D1::Matrix3x2F::Translation(m_CharacterPosition.x, m_CharacterPosition.y);
	// ����Ƽ ��ǥ�� ��ȯ�� ���� ���� Y�� ����
	D2D1::Matrix3x2F renderTransform = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	// ī�޶� ���� ��ǥ�� ��ȯ�� ���� ����� ���
	D2D1::Matrix3x2F cameraInv = D2D1::Matrix3x2F::Translation(m_CameraPosition.x, m_CameraPosition.y);
	cameraInv.Invert();

	// ��ǥ�� �� �׸���
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(cameraInv * m_UnityScreen);
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(-m_clientWidth), D2D1::Point2F(+m_clientWidth), m_brush.Get(), 1.0f);
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(0.0f, -m_clientHeight), D2D1::Point2F(0.0f, m_clientHeight), m_brush.Get(), 1.0f);

	// �ִϸ��̼� �̸� ǥ��
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	//std::wstring wMessage = L"Select Animation 1~9.   ,0: BasePose";
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawTextW(wMessage.c_str(), (UINT32)wMessage.length(),
	//	m_textFormat.Get(), D2D1::RectF(0, 0, 300, 10), m_brush.Get());
	//std::wstring wAnimName(m_currentAnimation.begin(), m_currentAnimation.end());
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawTextW(wAnimName.c_str(), (UINT32)wAnimName.length(),
	//	m_textFormat.Get(), D2D1::RectF(0, 20, 100, 30), m_brush.Get());

	// ���Ժ��� ������
	const auto& drawOrder = m_skeleton->getDrawOrder();
	for (size_t i = 0; i < drawOrder.size(); ++i)
	{
		spine::Slot* slot = drawOrder[i];
		spine::Attachment* attachment = slot->getAttachment();
		if (!attachment)
			continue;

		if (!attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
			continue; // RegionAttachment�� ó��

		spine::RegionAttachment* regionAtt = static_cast<spine::RegionAttachment*>(attachment);
		spine::TextureRegion* region = regionAtt->getRegion();
		if (!region)
			continue;

		spine::AtlasRegion* atlasRegion = static_cast<spine::AtlasRegion*>(region);
		if (!atlasRegion)
			continue;

		ID2D1Bitmap* bitmap = reinterpret_cast<ID2D1Bitmap*>(region->rendererObject);
		if (!bitmap)
			continue;

		// region.rotate Ȯ��
		assert(atlasRegion->degrees == 0 || atlasRegion->degrees == 90);

		// �ؽ�ó �̹��� ��ġ ����ȭ�� �̹����� ȸ���� ���� ����ó���� �ʿ��ϴ�.
		bool rotated = (atlasRegion->degrees == 90);
		float srcW = rotated ? atlasRegion->height : atlasRegion->width;
		float srcH = rotated ? atlasRegion->width : atlasRegion->height;
		D2D1_RECT_F srcRect;
		srcRect.left = (float)atlasRegion->x;
		srcRect.top = (float)atlasRegion->y;
		srcRect.right = srcRect.left + srcW;
		srcRect.bottom = srcRect.top + srcH;

		// 2. destRect + offset ����
		float destW = rotated ? atlasRegion->originalHeight : atlasRegion->originalWidth;
		float destH = rotated ? atlasRegion->originalWidth : atlasRegion->originalHeight;
		float offsetX = rotated ? atlasRegion->offsetY : atlasRegion->offsetX;
		float offsetY = rotated ? atlasRegion->offsetX : atlasRegion->offsetY;
		float attachX = regionAtt->getX() + offsetX;
		float attachY = regionAtt->getY() + offsetY;
		float attachRot = regionAtt->getRotation() + (rotated ? -90.0f : 0.0f);

		D2D1::Matrix3x2F attachmentMatrix =
			D2D1::Matrix3x2F::Scale(regionAtt->getScaleX(), regionAtt->getScaleY()) *
			D2D1::Matrix3x2F::Rotation(attachRot) *
			D2D1::Matrix3x2F::Translation(attachX, attachY);

		D2D1_RECT_F destRect;
		destRect.left = -destW / 2;
		destRect.top = -destH / 2;
		destRect.right = destRect.left + destW;
		destRect.bottom = destRect.top + destH;

		// 4. ��(Bone)�� ���� ��ȯ
		spine::Bone& bone = slot->getBone();
		D2D1::Matrix3x2F boneWorldMatrix =
			D2D1::Matrix3x2F::Scale(bone.getScaleX(), bone.getScaleY()) *
			D2D1::Matrix3x2F::Rotation(bone.getWorldRotationX()) *
			D2D1::Matrix3x2F::Translation(bone.getWorldX(), bone.getWorldY());

		D2D1::Matrix3x2F finalMatrix = renderTransform * attachmentMatrix * boneWorldMatrix * worldTransform * cameraInv * m_UnityScreen;
		D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(finalMatrix);

		D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 255, 0, 255);

		// Ȯ�ο� ���� �׸���
		//if (rotated)
		//	m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue, 0.1f));
		//else
		//	m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Green, 0.1f));
		//D2DRenderManager::GetInstance().m_d2dDeviceContext->FillRectangle(destRect, m_brush.Get());

		// �̹��� �׸���
		D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawBitmap(bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
	}

	// ���� ������ + ���ΰ� �̸� ǥ��
	if (m_skeleton)
	{
		auto& bones = m_skeleton->getBones();
		//m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 1.0f));
		for (size_t i = 0; i < bones.size(); ++i) {
			spine::Bone& bone = *bones[i];
			D2D1::Matrix3x2F boneWorldMatrix =
				D2D1::Matrix3x2F::Scale(bone.getScaleX(), bone.getScaleY()) *
				D2D1::Matrix3x2F::Rotation(bone.getWorldRotationX()) *
				D2D1::Matrix3x2F::Translation(bone.getWorldX(), bone.getWorldY());

			D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(renderTransform * boneWorldMatrix * worldTransform * cameraInv * m_UnityScreen);
			float x = 0, y = 0, crossLen = 8.0f;
			//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(x - crossLen, y), D2D1::Point2F(x + crossLen, y), m_brush.Get(), 2.0f);
			//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(x, y - crossLen), D2D1::Point2F(x, y + crossLen), m_brush.Get(), 2.0f);
			//std::wstring wBoneName(bone.getData().getName().buffer(), bone.getData().getName().buffer() + bone.getData().getName().length());
			//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawTextW(wBoneName.c_str(), (UINT32)wBoneName.length(),
			//	m_textFormat.Get(), D2D1::RectF(x + 10, y - 10, x + 100, y + 10), m_brush.Get());
		}
	}
}

// --- Ű���� �Է� ó�� ---
void SpineRenderer::HandleKeyInput(int keyCode) {
	std::cout << "Key pressed: " << keyCode << std::endl;
	// 1~9: �ش� �ε��� �ִϸ��̼� ���
	if (keyCode >= '1' && keyCode <= '9') {
		int idx = keyCode - '1';
		if (idx >= 0 && idx < (int)m_animationList.size()) {
			SetAnimation(m_animationList[idx]);
		}
		return;
	}
	switch (keyCode) {
	case '0': // 0�� Ű: �ִϸ��̼� ���� �⺻ ����
		if (m_state) m_state->clearTracks();
		if (m_skeleton) m_skeleton->setToSetupPose();
		SetAnimationTime(0.0f);
		break;
	case VK_LEFT: // ���� ȭ��ǥ: ���� �ִϸ��̼�
		SetPreviousAnimation();
		break;
	case VK_RIGHT: // ������ ȭ��ǥ: ���� �ִϸ��̼�
		SetNextAnimation();
		break;
	default:
		std::cout << "Unhandled key: " << keyCode << std::endl;
		break;
	}
}

void SpineRenderer::SetNextAnimation() {
	if (m_animationList.empty()) return;

	m_currentAnimationIndex = (m_currentAnimationIndex + 1) % m_animationList.size();
	std::string newAnimation = m_animationList[m_currentAnimationIndex];
	SetAnimation(newAnimation);

	std::cout << "Next animation: " << newAnimation << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
}

void SpineRenderer::SetPreviousAnimation() {
	if (m_animationList.empty()) return;

	m_currentAnimationIndex = (m_currentAnimationIndex - 1 + m_animationList.size()) % m_animationList.size();
	std::string newAnimation = m_animationList[m_currentAnimationIndex];
	SetAnimation(newAnimation);

	std::cout << "Previous animation: " << newAnimation << " (Index: " << m_currentAnimationIndex << ")" << std::endl;
}


bool SpineRenderer::LoadSpine(const std::string& atlasPath, const std::string& jsonPath)
{
	m_atlas.reset();
	m_atlas = std::make_unique<spine::Atlas>(atlasPath.c_str(), m_textureLoader.get());
	spine::SkeletonJson json(m_atlas.get());

	m_skeletonData.reset(json.readSkeletonDataFile(jsonPath.c_str())); // ���� new
	if (!m_skeletonData)
		return false;

	m_skeleton.reset(new spine::Skeleton(m_skeletonData.get()));
	m_stateData.reset(new spine::AnimationStateData(m_skeletonData.get()));
	m_state.reset(new  spine::AnimationState(m_stateData.get()));
	// �ִϸ��̼� ��� ä���
	m_animationList.clear();
	for (int i = 0; i < m_skeletonData->getAnimations().size(); ++i) {
		m_animationList.push_back(m_skeletonData->getAnimations()[i]->getName().buffer());
	}
	return true;
}

void SpineRenderer::ReleaseSpine()
{
	// spine-cpp ��ü�� ����
	m_state.reset();
	m_stateData.reset();
	m_skeleton.reset();
	m_skeletonData.reset();
	m_atlas.reset();
	m_spineBitmap.Reset();

	if (m_textureLoader)
		m_textureLoader->unload(nullptr); // ��Ʈ�� ����)
}

void SpineRenderer::ReleaseDirect2D()
{
	m_textureLoader.reset();
	m_brush.Reset();
	D2DRenderManager::GetInstance().m_d2dDeviceContext.Reset();
	m_factory.Reset();
	m_swapChain.Reset();
	m_deviceContext.Reset();
	m_device.Reset();
}


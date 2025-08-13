#include "pch.h"
#include "SpineRenderer.h"
#include <Extension/json.hpp>// nlohmann::json 사용
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
#include <Helpers/FileHelper.h>
#include <Helpers/StringHelper.h>
#include <System/RenderSystem.h>
#include <Manager/PackageResourceManager.h>

// Direct2DTextureLoader 구현
Direct2DTextureLoader::Direct2DTextureLoader(ID2D1RenderTarget* renderTarget)
	: m_renderTarget(renderTarget) {
}
Direct2DTextureLoader::~Direct2DTextureLoader() {}
void Direct2DTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
	std::wstring wPath(path.buffer(), path.buffer() + path.length());

	std::shared_ptr<ID2D1Bitmap1> bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(wPath.c_str());

	//Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
	//HRESULT hr = CoCreateInstance(
	//	CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	//if (FAILED(hr))
	//	return;
	//
	//Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	//hr = wicFactory->CreateDecoderFromFilename(wPath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	//if (FAILED(hr))
	//	return;
	//
	//Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	//hr = decoder->GetFrame(0, &frame);
	//if (FAILED(hr))
	//	return;
	//
	//Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	//hr = wicFactory->CreateFormatConverter(&converter);
	//if (FAILED(hr))
	//	return;
	//hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	//if (FAILED(hr))
	//	return;
	//
	//Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
	//
	//hr = D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), bitmap.GetAddressOf());
	//if (FAILED(hr))
	//	return;

	page.texture = bitmap.get();
	m_bitmapMap[path.buffer()] = bitmap;
	page.width = static_cast<int>(bitmap->GetSize().width);
	page.height = static_cast<int>(bitmap->GetSize().height);
}
void Direct2DTextureLoader::unload(void* texture)
{
	// ComPr 삭제
	m_bitmapMap.clear();
}


SpineRenderer::SpineRenderer() : m_textureLoader(nullptr), m_atlas(nullptr), m_skeletonData(nullptr), m_skeleton(nullptr), m_stateData(nullptr), m_state(nullptr) {}
SpineRenderer::~SpineRenderer() { Shutdown(); }

void SpineRenderer::RegistSystem(WeakObjectPtr<UObject> object)
{
	if (!ObjectHandler::GetInstance().IsValid(object.Get()->GetHandle())) return;
	
	RenderSystem::GetInstance().RegistSpine2D(object->GetHandle(), [this]() { Render(); }, GetDrawType(), GetLayer());
}

void SpineRenderer::Initialize()
{
	LoadTextureLoader();
	if (!m_textureLoader)
	{
		OutputDebugStringW(L"spineRenderer Initialize() Error!\n");
	}

}

void SpineRenderer::LoadTextureLoader()
{
	m_textureLoader = std::make_unique<Direct2DTextureLoader>(D2DRenderManager::GetInstance().m_d2dDeviceContext.Get());

	FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
	m_UnityScreen = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
		D2D1::Matrix3x2F::Translation((float)screenSize.x / 2, (float)screenSize.y / 2);
}

void SpineRenderer::Shutdown()
{
	ReleaseSpine();
	ReleaseDirect2D();
}

void SpineRenderer::ReleaseResources() {

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
	FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
	return D2D1::SizeF(static_cast<float>(screenSize.x), static_cast<float>(screenSize.y)); // 기본 크기
}

void SpineRenderer::Clear(const D2D1_COLOR_F& color) {
	if (!m_initialized) return;

	D2DRenderManager::GetInstance().m_d2dDeviceContext->Clear(color);
}



// --- 슬롯별 이미지 렌더링 ---
void SpineRenderer::Render()
{
	if (!m_bRendered) return;
	if (!m_skeleton || !m_atlas || !D2DRenderManager::GetInstance().m_d2dDeviceContext)
		return;

	D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);

	// 캐릭터의 월드 위치
	D2D1::Matrix3x2F worldTransform = D2D1::Matrix3x2F::Translation(m_CharacterPosition.x, m_CharacterPosition.y);
	// 유니티 좌표계 변환을 위해 사전 Y축 반전
	D2D1::Matrix3x2F renderTransform = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	// 카메라 기준 좌표계 변환을 위해 역행렬 계산
	D2D1::Matrix3x2F cameraInv = D2D1::Matrix3x2F::Translation(m_CameraPosition.x, m_CameraPosition.y);
	cameraInv.Invert();

	// 좌표계 축 그리기
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(cameraInv * m_UnityScreen);
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(-m_clientWidth), D2D1::Point2F(+m_clientWidth), m_brush.Get(), 1.0f);
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawLine(D2D1::Point2F(0.0f, -m_clientHeight), D2D1::Point2F(0.0f, m_clientHeight), m_brush.Get(), 1.0f);

	// 애니메이션 이름 표시
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	//std::wstring wMessage = L"Select Animation 1~9.   ,0: BasePose";
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawTextW(wMessage.c_str(), (UINT32)wMessage.length(),
	//	m_textFormat.Get(), D2D1::RectF(0, 0, 300, 10), m_brush.Get());
	//std::wstring wAnimName(m_currentAnimation.begin(), m_currentAnimation.end());
	//D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawTextW(wAnimName.c_str(), (UINT32)wAnimName.length(),
	//	m_textFormat.Get(), D2D1::RectF(0, 20, 100, 30), m_brush.Get());

	// 슬롯별로 렌더링
	const auto& drawOrder = m_skeleton->getDrawOrder();
	for (size_t i = 0; i < drawOrder.size(); ++i)
	{
		spine::Slot* slot = drawOrder[i];
		spine::Attachment* attachment = slot->getAttachment();
		if (!attachment)
			continue;

		if (!attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
			continue; // RegionAttachment만 처리

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

		// region.rotate 확인
		assert(atlasRegion->degrees == 0 || atlasRegion->degrees == 90);

		// 텍스처 이미지 배치 최적화로 이미지가 회전된 경우는 예외처리가 필요하다.
		bool rotated = (atlasRegion->degrees == 90);
		float srcW = rotated ? atlasRegion->height : atlasRegion->width;
		float srcH = rotated ? atlasRegion->width : atlasRegion->height;
		D2D1_RECT_F srcRect;
		srcRect.left = (float)atlasRegion->x;
		srcRect.top = (float)atlasRegion->y;
		srcRect.right = srcRect.left + srcW;
		srcRect.bottom = srcRect.top + srcH;

		// 2. destRect + offset 적용
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

		// 4. 본(Bone)의 월드 변환
		spine::Bone& bone = slot->getBone();
		D2D1::Matrix3x2F boneWorldMatrix =
			D2D1::Matrix3x2F::Scale(bone.getScaleX(), bone.getScaleY()) *
			D2D1::Matrix3x2F::Rotation(bone.getWorldRotationX()) *
			D2D1::Matrix3x2F::Translation(bone.getWorldX(), bone.getWorldY());

		D2D1::Matrix3x2F finalMatrix = renderTransform * attachmentMatrix * boneWorldMatrix * worldTransform * cameraInv * m_UnityScreen;
		D2DRenderManager::GetInstance().m_d2dDeviceContext->SetTransform(finalMatrix);

		D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 255, 0, 255);

		// 확인용 영역 그리기
		//if (rotated)
		//	m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue, 0.1f));
		//else
		//	m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Green, 0.1f));
		//D2DRenderManager::GetInstance().m_d2dDeviceContext->FillRectangle(destRect, m_brush.Get());
		
		// 이미지 그리기
		D2DRenderManager::GetInstance().m_d2dDeviceContext->DrawBitmap(bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
	}

	// 본의 원점에 + 라인과 이름 표시
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

// --- 키보드 입력 처리 ---
void SpineRenderer::HandleKeyInput(int keyCode) {
	std::cout << "Key pressed: " << keyCode << std::endl;
	// 1~9: 해당 인덱스 애니메이션 재생
	if (keyCode >= '1' && keyCode <= '9') {
		int idx = keyCode - '1';
		if (idx >= 0 && idx < (int)m_animationList.size()) {
			SetAnimation(m_animationList[idx]);
		}
		return;
	}
	switch (keyCode) {
	case '0': // 0번 키: 애니메이션 없이 기본 포즈
		if (m_state) m_state->clearTracks();
		if (m_skeleton) m_skeleton->setToSetupPose();
		SetAnimationTime(0.0f);
		break;
	case VK_LEFT: // 왼쪽 화살표: 이전 애니메이션
		SetPreviousAnimation();
		break;
	case VK_RIGHT: // 오른쪽 화살표: 다음 애니메이션
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


void SpineRenderer::LoadSpine(const std::wstring& atlasPath, const std::wstring& jsonPath)
{
	std::wstring atlasFilePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + atlasPath); // 파일 이름만 저장
	std::wstring jsonFilePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + jsonPath); // 파일 이름만 저장
	std::string atlasFileStrPath = StringHelper::wstring_to_string(atlasFilePath);
	std::string jsonFileStrPath = StringHelper::wstring_to_string(jsonFilePath);

	m_atlas.reset();
	m_atlas = std::make_unique<spine::Atlas>(atlasFileStrPath.c_str(), m_textureLoader.get());
	spine::SkeletonJson json(m_atlas.get());

	m_skeletonData.reset(json.readSkeletonDataFile(jsonFileStrPath.c_str())); // 내부 new
	if (!m_skeletonData)
	{
		OutputDebugStringW(L"spineRenderer LoadSpine()  m_skeletonDataError!\n");
		return;
	}

	m_skeleton.reset(new spine::Skeleton(m_skeletonData.get()));
	m_stateData.reset(new spine::AnimationStateData(m_skeletonData.get()));
	m_state.reset(new  spine::AnimationState(m_stateData.get()));
	// 애니메이션 목록 채우기
	m_animationList.clear();
	for (int i = 0; i < m_skeletonData->getAnimations().size(); ++i) {
		m_animationList.push_back(m_skeletonData->getAnimations()[i]->getName().buffer());
	}

	m_initialized = true;
}

void SpineRenderer::ReleaseSpine()
{
	// spine-cpp 객체들 해제
	m_state.reset();
	m_stateData.reset();
	m_skeleton.reset();
	m_skeletonData.reset();
	m_atlas.reset();

	if (m_textureLoader)
		m_textureLoader->unload(nullptr); // 비트맵 해제)
}

void SpineRenderer::ReleaseDirect2D()
{
	m_textureLoader.reset();
}


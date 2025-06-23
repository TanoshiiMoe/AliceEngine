#include <spine/spine.h>
#include <spine/spine-alice.h>
#include "pch.h"
#include "Spine2DRenderComponent.h"
#include "Manager/D2DRenderManager.h"
#include <spine/Spine2DTextureLoader.h>

using namespace spine;

void Spine2DRenderComponent::Initialize()
{
}

void Spine2DRenderComponent::Update()
{
}

void Spine2DRenderComponent::UpdateFrames()
{
}

void Spine2DRenderComponent::ReleaseFrames()
{
}

void Spine2DRenderComponent::LoadData(const std::wstring& path)
{
	ID2D1Bitmap1* _bitmap = nullptr;
	D2DRenderManager::Get().CreateBitmapFromFile(path.c_str(), &_bitmap);
	//D2DSpine2DTextureLoader* textureLoader = new D2DSpine2DTextureLoader(_bitmap);
	// 1. Atlas 로드
	//Atlas* atlas = new Atlas("Resource\\yuuka_spr\\yuuka_spr.atlas", textureLoader);

	//SkeletonDrawable_D2D* skeletonDrawable = new SkeletonDrawable_D2D(_bitmap, L"Resource\\yuuka_spr\\yuuka_spr");
	
	//// 2. SkeletonBinary 인스턴스 생성
	//SkeletonBinary binary(atlas);
	//binary.setScale(1.0f); // 스케일 조정
	//
	//// 3. .skel 파일 로드
	//spine::SkeletonData* skeletonData = binary.readSkeletonDataFile("Resource\\yuuka_spr\\yuuka_spr.skel");
	//if (!skeletonData) {
	//	// 에러 처리: binary.getError().buffer()
	//}
}

void Spine2DRenderComponent::LoadFrame(size_t frameIndex)
{
}

void Spine2DRenderComponent::Release()
{
}

void Spine2DRenderComponent::Render()
{
}

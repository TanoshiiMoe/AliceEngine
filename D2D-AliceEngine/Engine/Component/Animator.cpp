#include "pch.h"
#include "Animator.h"
#include <Math/Transform.h>
#include <Helpers/TextureLoader.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/StringHelper.h>
#include <Application.cpp>

Animator::Animator()
{
	pAnimationClip = new AnimationClip();
}

Animator::~Animator()
{
	delete pAnimationClip;
}

void Animator::Initialize()
{
	__super::Initialize();
}

void Animator::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (bPlay)	// 플레이 가능할 때만 플레이
	{
		m_fFPSLastTime = deltaSeconds - m_fcountOneSecond;
		if (m_fFPSLastTime >= m_fFPSTime)	// 1/60 초에 한 번씩
		{
			if (pAnimationClip->frames.size() != 0)
			{
				m_curClip = (m_curClip + 1) % pAnimationClip->frames.size();
			}
			m_fcountOneSecond = deltaSeconds;
		}
	}
}

void Animator::Release()
{
	__super::Release();
}

void Animator::Render()
{
	__super::Render();

	if (sheet.lock() == nullptr) return;
	if (m_bitmap == nullptr) return;
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize(); // 비트맵 크기 및 피벗

	ComPtr<ID2D1Effect> cropEffect;
	context->CreateEffect(CLSID_D2D1Crop, &cropEffect);
	cropEffect->SetInput(0, m_bitmap.get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, D2D1::RectF(sheet.lock()->sprites[m_curClip].x, sheet.lock()->sprites[m_curClip].y, sheet.lock()->sprites[m_curClip].width, sheet.lock()->sprites[m_curClip].height));
	D2D1_POINT_2F pivotOffset = {
		sheet.lock()->sprites[m_curClip].width * sheet.lock()->sprites[m_curClip].pivotX,
		sheet.lock()->sprites[m_curClip].height * sheet.lock()->sprites[m_curClip].pivotY
	};
	//pivotOffset = { 0,0 };

	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = GetTransform()->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// 로컬 피벗 기준 월드 변환, 카메라 역행렬 적용
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity 좌표계면 변환 추가
	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// 최종 변환 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	context->SetTransform(view);

	D2D1_VECTOR_2F SrcPos = { 0, 0 };
	D2D1_RECT_F SrcRect = { SrcPos.x, SrcPos.y, SrcPos.x + bmpSize.width, SrcPos.y + bmpSize.height };
	
	SrcPos = { sheet.lock()->sprites[m_curClip].x, sheet.lock()->sprites[m_curClip].y };
	//SrcRect = { SrcPos.x, SrcPos.y, SrcPos.x + spriteInfo.width, SrcPos.y + spriteInfo.height };
	//SrcRect = { SrcPos.x, SrcPos.y, SrcPos.x + sheet.lock()->sprites[m_curClip].width, SrcPos.y + sheet.lock()->sprites[m_curClip].height };
	SrcRect = { SrcPos.x, SrcPos.y, SrcPos.x + 160, SrcPos.y + 160 };
	context->DrawBitmap(m_bitmap.get(), nullptr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
	//context->DrawImage(cropEffect.Get());
}

void Animator::SetAnimationClip(AnimationClip* clip)
{
	pAnimationClip = clip;
}

void Animator::PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip)
{

}

void Animator::LoadSpriteData(const std::string& filePath)
{
}

void Animator::LoadSpriteSheet(const std::string& filePath, std::weak_ptr<SpriteSheet> spriteSheet)
{
	if (spriteSheet.lock())
	{
		TextureLoader::LoadSpriteSheet(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(filePath))), *spriteSheet.lock());
	}
}

void Animator::LoadAnimationClip(const std::string& filePath, std::weak_ptr<AnimationClip> clip, const std::weak_ptr<SpriteSheet> spriteSheet)
{
	if (spriteSheet.lock())
		TextureLoader::LoadAnimationClip(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(filePath))), *clip.lock(), *spriteSheet.lock());
}

void Animator::LoadSpriteRenderer(std::weak_ptr<SpriteSheet> _sheet)
{
	const std::wstring path = StringHelper::string_to_wstring(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(pAnimationClip->texturePath))));
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(path.c_str());
	sheet = _sheet;
}

void Animator::Play()
{
	bPlay = true;
}

void Animator::Stop()
{
	bPlay = false;
}

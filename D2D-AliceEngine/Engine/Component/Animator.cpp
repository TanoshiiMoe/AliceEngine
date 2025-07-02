#include "pch.h"
#include "Animator.h"
#include <Math/Transform.h>
#include <Helpers/TextureLoader.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/StringHelper.h>
#include <Application.cpp>
#include <Math/TColor.h>

Animator::Animator()
{
}

Animator::~Animator()
{

}

void Animator::Initialize()
{
	__super::Initialize();

	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void Animator::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!bPlay) return;	// 플레이 가능할 때만 플레이
	if (!pAnimationClip || pAnimationClip->frames.empty()) return;
	if (IsEnd() && !bLoopping) return; // 프레임의 끝에서 반복할지 말지 판단

	m_accumTime += deltaSeconds;
	if (m_accumTime >= pAnimationClip->frames[m_curFrame].duration)
	{
		m_accumTime = pAnimationClip->frames[m_curFrame].duration;
		m_curFrame = (m_curFrame + 1) % pAnimationClip->frames.size();
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
	auto& sprite = sheet.lock()->sprites[pAnimationClip->frames[m_curFrame].spriteSheetIndex];

	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();
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
	float ScaleX = bmpSize.width / sheet.lock()->textureWidth;
	float ScaleY = bmpSize.height / sheet.lock()->textureHeight;
	float x = sprite.x * ScaleX;
	float y = bmpSize.height - sprite.y * ScaleY;
	float width = sprite.width * ScaleX;
	float height = sprite.height * ScaleY;
	D2D1_RECT_F SrcRect = { x, y - height, x + width, y };
	D2D1_RECT_F destRect = { -width * sprite.pivotX, -height * sprite.pivotY,  -width * sprite.pivotX + width,  -height * sprite.pivotY + height };

	if (bFlip)
	{
		D2D1::Matrix3x2F flipX =
			D2D1::Matrix3x2F::Scale(-1.f, 1.f);
		view = flipX * view;
	}
	context->SetTransform(view);
	context->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
	D2DRenderManager::GetInstance().DrawDebugBox(-10, -10, 10, 10, 0, 0, 0, 255);
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
	//const std::wstring path = StringHelper::string_to_wstring(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(pAnimationClip->texturePath))));
	const std::wstring path = StringHelper::string_to_wstring(StringHelper::wstring_to_string(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(_sheet.lock()->texture))));
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(path.c_str());
	sheet = _sheet;
}

void Animator::Play()
{
	if (!pAnimationClip) return;
	bPlay = true;
}

void Animator::Stop()
{
	if (!pAnimationClip) return;
	m_curFrame = 0;
	bPlay = false;
}

void Animator::PlayAtFrame(const size_t& _frame)
{
	if (!pAnimationClip) return;
	m_curFrame = _frame;
	bPlay = true;
}

void Animator::PlayAtStart()
{
	if (!pAnimationClip) return;
	m_curFrame = 0;
	bPlay = true;
}

void Animator::StopAndResetFrame()
{
	if (!pAnimationClip) return;
	bPlay = false;
}

bool Animator::IsEnd()
{
	if (!pAnimationClip) return false;
	return m_curFrame >= pAnimationClip->frames.size() - 1;
}

void Animator::ChangeAnimation(const std::string& _key, bool _loop)
{

}

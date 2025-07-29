#include "pch.h"
#include "Prism.h"

#include "Object/gameObject.h"
#include "Component/SpriteRenderer.h"
#include "Manager/SceneManager.h"
#include "Component/TransformComponent.h"
#include "Manager/UpdateTaskManager.h"
#include "Manager/D2DRenderManager.h"

Prism::~Prism()
{
	while (!objects.empty()) {
		objects.front().reset();
		objects.pop_front();
	}
}

void Prism::Initialize()
{
	//WeakFromThis<Prism>()

	if (owner)
		animator = owner->GetComponent<Animator>();
	else {
		OutputDebugStringW(L"프리즘 컴포넌트에서 Animator를 찾을수 없습니다!!");
		return;
	}

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_LastDemotable,
		[weak = WeakFromThis<ITickable>()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);

}

void Prism::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 애니메이터 없을시 리턴
	if (animator == nullptr)
		return;

	ellipsedTime += deltaSeconds;

	if (ellipsedTime >= interval) {
		ellipsedTime -= interval;

		// TODO::산데비스탄 생성
		MakeEffect();
	}
}

void Prism::GetSpriteInfo()
{
	// 애니메이터 없을시 리턴
	if (animator == nullptr) {
		return;
	}

	//AnimationClip* currClip = animator->animationClips[animator->curAnimationClip];
	FrameInfo* frameInfo = &(animator->animationClips[animator->curAnimationClip]->frames[animator->m_curFrame]);
	int spriteIndex = animator->sheet->spriteIndexMap[frameInfo->sprite];
	spriteInfo = &(animator->sheet->sprites[spriteIndex]);
}


void Prism::GetCurrBitmap()
{
	// 애니메이터 없을시 리턴
	if (animator == nullptr) {
		return;
	}
	
	bitmap = animator->m_bitmap;
}

void Prism::MakeEffect()
{
	GetSpriteInfo();
	GetCurrBitmap();

	if (!owner) {
		OutputDebugStringW(L"프리즘 컴포넌트에서 owner를 찾을수 없습니다!!");
		return;
	}
		

	if (spriteInfo == nullptr) {
		OutputDebugStringW(L"프리즘 컴포넌트에서 spriteInfo를 찾을수 없습니다!!");
		return;
	}

	if (bitmap.expired()) {
		OutputDebugStringW(L"프리즘 컴포넌트에서 bitmap을 찾을수 없습니다!!");
		return;
	}



	WeakObjectPtr<gameObject> temp = SceneManager::GetInstance().m_currentScene->NewObject<gameObject>(L"effect");
	if (temp) {
		std::wstring tt2 = L"-------\n";
		OutputDebugStringW(tt2.c_str());
		// 스프라이트 렌더러 설정
		SpriteRenderer* sr = temp->AddComponent<SpriteRenderer>();
		sr->m_bitmap = bitmap.lock();
		sr->m_layer = animator->m_layer;
		sr->SetFlip(animator->bFlip);
		// y값에 왜 Height만큼 빼야함????????????????? <- x,y 점이 좌하단에 찍히기 때문에 - height 해야 정상화된다.
		sr->SetSlice(spriteInfo->x, spriteInfo->y - spriteInfo->height, spriteInfo->width, spriteInfo->height);
		//sr->spriteInfo = *spriteInfo;

		// 트랜스폼 설정
		// TODO::localTransform 오류 해결되면 SetWorldTransform 제거하기
		temp->transform()->SetWorldPosition(owner->transform()->GetPosition());
		//temp->transform()->SetPosition(owner->transform()->GetPosition());
		temp->transform()->SetRotation(owner->transform()->GetRotation());
		temp->transform()->SetWorldScale(owner->transform()->GetScale());
		//temp->transform()->SetScale(owner->transform()->GetScale().x, owner->transform()->GetScale().y);
		// 
		// 임시 오브젝트 큐에 저장
		objects.push_back(temp);

		// 갯수 넘어가면 삭제
		while (objects.size() > prismCount) {
			SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
			objects.front().reset();
			objects.pop_front();
		}

		// 오브젝트들 렌더순서 변경
		for (auto& object : objects) {
			if (object) {
				object->GetComponent<SpriteRenderer>()->m_layer -= 1;
			}
		}

		// 색깔필터 변경하기
		ChangeColor();
	}
	else {
		OutputDebugStringW(L"프리즘 컴포넌트에서 gameObject를 생성할수 없습니다!!");
		return;
	}
}

void Prism::ChangeColor()
{
	int it = 0;

	for (auto& object : objects) {
		if (object) {
			SpriteRenderer* sr = object->GetComponent<SpriteRenderer>();
			if (sr) {
				ComPtr<ID2D1Effect> colorEffect;
				D2DRenderManager::GetD2DDevice()->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect);

				// 원본 이미지 입력
				colorEffect->SetInput(0, sr->m_bitmap.get());
				
				float red = (1.0f / (float)prismCount) * (prismCount - it);
				float blue = (1.0f / (float)prismCount) * it;
				float alpha = 0.8f - red;

				// 색상 행렬 설정 (예 : 빨강 강조)
				D2D1_MATRIX_5X4_F colorMatrix = {
					red, 0.0f, blue, 0.0f, // R
					0.0f, 0.5f, 0.0f, 0.0f, // G
					red, 0.0f, blue, 0.0f, // B
					0.0f, 0.0f, 0.0f, alpha,	// A
					0.0f, 0.0f, 0.0f, 0.0f	// 1
				};

				colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
				
				sr->m_effect = colorEffect;
			}
			else {
				OutputDebugStringW(L"프리즘 컴포넌트에서 object를 찾을수 없습니다!!");
			}
		}
		++it;
	}
}
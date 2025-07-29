#include "pch.h"
#include "Prism.h"

#include "Object/gameObject.h"
#include "Component/SpriteRenderer.h"
#include "Manager/SceneManager.h"
#include "Component/TransformComponent.h"
#include "Manager/UpdateTaskManager.h"

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
		// TODO::스프라이터 렌더러에 srcRect 설정하기
		sr->SetSlice(spriteInfo->x, (spriteInfo->y / 1.5f) , spriteInfo->width, spriteInfo->height);
		//sr->spriteInfo = *spriteInfo;

		// 트랜스폼 설정
		temp->transform()->SetWorldPosition(owner->transform()->GetPosition());
		//temp->transform()->SetPosition(owner->transform()->GetPosition());
		temp->transform()->SetRotation(owner->transform()->GetRotation());
		temp->transform()->SetScale(owner->transform()->GetScale().x, owner->transform()->GetScale().y);
		std::wstring tt = L"11 temp->transform() " + std::to_wstring(temp->transform()->GetPosition().x) + L"\n";
		OutputDebugStringW(tt.c_str());
		// 임시 오브젝트 큐에 저장
		objects.push_back(temp);

		// 갯수 넘어가면 삭제
		while (objects.size() > prismCount) {
			// TODO::Delete 하기
			SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
			objects.front().reset();
			objects.pop_front();
		}

		// 오브젝트들 렌더순서 변경
		for (auto& object : objects) {
			if (object) {
				object->GetComponent<SpriteRenderer>()->m_layer -= 1;
				std::wstring tt = L"22 temp->transform() " + std::to_wstring(object->transform()->GetPosition().x) + L"\n";
				OutputDebugStringW(tt.c_str());
			}
		}

		// 색깔필터 변경하기
	}
	else {
		OutputDebugStringW(L"프리즘 컴포넌트에서 gameObject를 생성할수 없습니다!!");
		return;
	}
}

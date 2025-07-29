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
		OutputDebugStringW(L"������ ������Ʈ���� Animator�� ã���� �����ϴ�!!");
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

	// �ִϸ����� ������ ����
	if (animator == nullptr)
		return;

	ellipsedTime += deltaSeconds;

	if (ellipsedTime >= interval) {
		ellipsedTime -= interval;

		// TODO::�굥��ź ����
		MakeEffect();
	}
}

void Prism::GetSpriteInfo()
{
	// �ִϸ����� ������ ����
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
	// �ִϸ����� ������ ����
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
		OutputDebugStringW(L"������ ������Ʈ���� owner�� ã���� �����ϴ�!!");
		return;
	}
		

	if (spriteInfo == nullptr) {
		OutputDebugStringW(L"������ ������Ʈ���� spriteInfo�� ã���� �����ϴ�!!");
		return;
	}

	if (bitmap.expired()) {
		OutputDebugStringW(L"������ ������Ʈ���� bitmap�� ã���� �����ϴ�!!");
		return;
	}



	WeakObjectPtr<gameObject> temp = SceneManager::GetInstance().m_currentScene->NewObject<gameObject>(L"effect");
	if (temp) {
		std::wstring tt2 = L"-------\n";
		OutputDebugStringW(tt2.c_str());
		// ��������Ʈ ������ ����
		SpriteRenderer* sr = temp->AddComponent<SpriteRenderer>();
		sr->m_bitmap = bitmap.lock();
		sr->m_layer = animator->m_layer;
		// TODO::���������� �������� srcRect �����ϱ�
		sr->SetSlice(spriteInfo->x, (spriteInfo->y / 1.5f) , spriteInfo->width, spriteInfo->height);
		//sr->spriteInfo = *spriteInfo;

		// Ʈ������ ����
		temp->transform()->SetWorldPosition(owner->transform()->GetPosition());
		//temp->transform()->SetPosition(owner->transform()->GetPosition());
		temp->transform()->SetRotation(owner->transform()->GetRotation());
		temp->transform()->SetScale(owner->transform()->GetScale().x, owner->transform()->GetScale().y);
		std::wstring tt = L"11 temp->transform() " + std::to_wstring(temp->transform()->GetPosition().x) + L"\n";
		OutputDebugStringW(tt.c_str());
		// �ӽ� ������Ʈ ť�� ����
		objects.push_back(temp);

		// ���� �Ѿ�� ����
		while (objects.size() > prismCount) {
			// TODO::Delete �ϱ�
			SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
			objects.front().reset();
			objects.pop_front();
		}

		// ������Ʈ�� �������� ����
		for (auto& object : objects) {
			if (object) {
				object->GetComponent<SpriteRenderer>()->m_layer -= 1;
				std::wstring tt = L"22 temp->transform() " + std::to_wstring(object->transform()->GetPosition().x) + L"\n";
				OutputDebugStringW(tt.c_str());
			}
		}

		// �������� �����ϱ�
	}
	else {
		OutputDebugStringW(L"������ ������Ʈ���� gameObject�� �����Ҽ� �����ϴ�!!");
		return;
	}
}

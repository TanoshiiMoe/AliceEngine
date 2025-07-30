#include "pch.h"
#include "Prism.h"

#include "Object/gameObject.h"
#include "Component/SpriteRenderer.h"
#include "Manager/SceneManager.h"
#include "Component/TransformComponent.h"
#include "Manager/UpdateTaskManager.h"
#include "Manager/D2DRenderManager.h"
#include <Component/Rigidbody2D.h>

Prism::~Prism()
{
	ClearObjects();
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

	if (!isActive)
		return;

	// ��Ƽ�� �� ���� isEnabled�� false��
	if (isEnabled && isDirty && objects.size() == prismCount)
		isEnabled = false;

	// count || interval�� 0���� ������ �������
	if (prismCount <= 0 || interval <= 0.0f) {
		isActive = false;
		return;
	}

	// �ִϸ����� ������ ����
	if (animator == nullptr) {
		isActive = false;
		return;
	}
		
	ellipsedTime += deltaSeconds;

	if (ellipsedTime >= interval) {
		ellipsedTime -= interval;

		// TODO::�굥��ź ����
		if(isEnabled)
			MakeEffect();
		ChangeColor();
		DeleteLast();
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
		// ��������Ʈ ������ ����
		SpriteRenderer* sr = temp->AddComponent<SpriteRenderer>();
		sr->m_bitmap = bitmap.lock();
		sr->m_layer = animator->m_layer;
		sr->SetFlip(animator->bFlip);
		// y���� �� Height��ŭ ������????????????????? <- x,y ���� ���ϴܿ� ������ ������ - height �ؾ� ����ȭ�ȴ�.
		sr->SetSlice(spriteInfo->x, spriteInfo->y - spriteInfo->height, spriteInfo->width, spriteInfo->height);
		//sr->spriteInfo = *spriteInfo;

		temp->AddComponent<Rigidbody2D>();
		if (auto rb = temp->GetComponent<Rigidbody2D>())
		{
			rb->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
		}

		// Ʈ������ ����
		// TODO::localTransform ���� �ذ�Ǹ� SetWorldTransform �����ϱ�
		temp->transform()->SetPosition(owner->transform()->GetPosition());
		temp->transform()->SetRotation(owner->transform()->GetRotation());
		//temp->transform()->SetWorldScale(owner->transform()->GetScale());
		temp->transform()->SetScale(owner->transform()->GetScale().x, owner->transform()->GetScale().y);
		// 
		// �ӽ� ������Ʈ ť�� ����
		objects.push_back(temp);

		// ������Ʈ�� �������� ����
		for (auto& object : objects) {
			if (object) {
				object->GetComponent<SpriteRenderer>()->m_layer -= 1;
			}
		}
	}
	else {
		OutputDebugStringW(L"������ ������Ʈ���� gameObject�� �����Ҽ� �����ϴ�!!");
		return;
	}
}

void Prism::ChangeColor()
{
	// �ڿ������� �����ؾ���
	auto it = objects.rbegin();

	while (it != objects.rend()) {
		if (*it) {
			SpriteRenderer* sr = (*it)->GetComponent<SpriteRenderer>();
			if (sr) {
				ComPtr<ID2D1Effect> colorEffect;

				D2DRenderManager::GetD2DDevice()->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect);

				//���� �̹��� �Է�
				colorEffect->SetInput(0, sr->m_bitmap.get());
				
				int distance = std::distance(objects.rbegin(), it);

				float blue = 0.0f;
				float red = 0.0f;
				float alpha = 0.0f;

				// ���� enable�� ���� ���ϱ�
				if (isEnabled) {
					blue = (1.0f / (float)prismCount) * (prismCount - distance);
					red = (1.0f / (float)prismCount) * distance;
					alpha = 0.8f - red;
				}
				else {
					blue = (1.0f / (float)prismCount) * (prismCount - (distance + (prismCount - objects.size())));
					red = (1.0f / (float)prismCount) * (distance + (prismCount - objects.size()));
					alpha = 0.8f - red;
				}


				// ���� ��� ���� (�� : ���� ����)
				D2D1_MATRIX_5X4_F colorMatrix = {
					red, 0.0f, blue, 0.0f, // R
					0.0f, 0.5f, 0.0f, 0.0f, // G
					red, 0.0f, blue, 0.0f, // B
					0.0f, 0.0f, 0.0f, alpha,	// A
					0.0f, 0.0f, 0.0f, 0.0f	// 1
				};

				colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);

				sr->m_effect = colorEffect;
				//sr->m_effect->SetInputEffect(0, colorEffect.Get());
			}
			else {
				OutputDebugStringW(L"������ ������Ʈ���� object�� ã���� �����ϴ�!!");
			}
		}
		++it;
	}

	//int it = 0;

	//for (auto& object : objects) {
	//	if (object) {
	//		SpriteRenderer* sr = object->GetComponent<SpriteRenderer>();
	//		if (sr) {
	//			ComPtr<ID2D1Effect> colorEffect;
	//			D2DRenderManager::GetD2DDevice()->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect);

	//			// ���� �̹��� �Է�
	//			colorEffect->SetInput(0, sr->m_bitmap.get());
	//			
	//			float red = (1.0f / (float)prismCount) * (prismCount - it);
	//			float blue = (1.0f / (float)prismCount) * it;
	//			float alpha = 0.8f - red;

	//			// ���� ��� ���� (�� : ���� ����)
	//			D2D1_MATRIX_5X4_F colorMatrix = {
	//				red, 0.0f, blue, 0.0f, // R
	//				0.0f, 0.5f, 0.0f, 0.0f, // G
	//				red, 0.0f, blue, 0.0f, // B
	//				0.0f, 0.0f, 0.0f, alpha,	// A
	//				0.0f, 0.0f, 0.0f, 0.0f	// 1
	//			};

	//			colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
	//			
	//			sr->m_effect = colorEffect;
	//		}
	//		else {
	//			OutputDebugStringW(L"������ ������Ʈ���� object�� ã���� �����ϴ�!!");
	//		}
	//	}
	//	++it;
	//}
}

void Prism::DeleteLast()
{
	if (isEnabled) {
		// ���� �Ѿ�� ����
		while (objects.size() > prismCount) {
			SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
			objects.front().reset();
			objects.pop_front();
		}
	}
	else if (!objects.empty()) {
		// �տ��ͺ��� ���ʴ�� ����
		SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
		objects.front().reset();
		objects.pop_front();
	}
	else {
		isActive = false;
		ellipsedTime = 0.0f;
	}
}

void Prism::ClearObjects()
{
	while (!objects.empty()) {
		SceneManager::GetInstance().GetWorld()->RemoveObject(objects.front().Get());
		objects.front().reset();
		objects.pop_front();
	}
	objects.clear();
	ellipsedTime = 0.0f;
}

void Prism::SetActive(bool _val)
{
	isDirty = !(_val);

	if (_val == true) {
		isDirty = false;
		isEnabled = true;
		isActive = true;
		ClearObjects();
	}
}

bool Prism::IsActive()
{
	return isEnabled;
}


#include "pch.h"
#include "ButtonComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Manager/UpdateTaskManager.h>
#include <Core/Input.h>
#include <Helpers/FileHelper.h>
#include <Helpers/CoordHelper.h>

ButtonComponent::ButtonComponent()
{
	drawType = Define::EDrawType::ScreenSpace;
}

ButtonComponent::~ButtonComponent()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // ��Ʈ�� ��ε�
}

void ButtonComponent::Initialize()
{
	__super::Initialize();

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostUpdateWork);
}

void ButtonComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	if (!bActive) return;

	bool mouseDown = Input::IsMouseLeftDown();
	bool mouseUp = Input::IsMouseLeftReleased();
	FVector2 mousePos = Input::GetMouseWorldPositionInCanvas();
	FVector2 relativePos = FVector2(
		relativeTransform.GetPosition().x,
		relativeTransform.GetPosition().y
	);

	FVector2 uiSize = GetRelativeSize();
	FVector2 finalUIPos = relativePos + FVector2(-uiSize.x * GetOwnerPivot()->x, -uiSize.y * GetOwnerPivot()->y);

	// UI ���� �� ���콺 Ŭ�� Ȯ��
	if (IsMouseInUIArea(mousePos, finalUIPos, uiSize))
	{
		// ���콺�� UI ���� �ȿ� ���� ���� �Լ� ȣ��
		SetCursor(LoadCursorW(nullptr, IDC_HAND));

		if (mouseUp && m_prevMouseDown) // ���콺�� �������� ������ �����־��ٸ� Release ����
		{
			SetCurrentState(EButtonState::Release);
		}
		else if (mouseDown) // ���콺�� ���������� Pressed ����
		{
			SetCurrentState(EButtonState::Pressed);
		}
		else
		{
			SetCurrentState(EButtonState::Hover); // ���콺�� �ö������� ������ �ʾ����� Hover ����
		}
	}
	else
	{
		SetCurrentState(EButtonState::Idle);
		SetCursor(LoadCursorW(nullptr, IDC_ARROW));
	}

	m_prevMouseDown = mouseDown;
}

void ButtonComponent::Release()
{
	__super::Release();
	stateActionSlots.clear();
}

void ButtonComponent::Render()
{
	if (!m_bitmap) return;
	__super::Render();

	FVector2 relativeSize = FVector2(GetBitmapSizeX(), GetBitmapSizeY());
	D2D1_RECT_F destRect = D2D1::RectF(-relativeSize.x / 2, -relativeSize.y / 2, relativeSize.x / 2, relativeSize.y / 2);
	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), destRect);
}

float ButtonComponent::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float ButtonComponent::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 ButtonComponent::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void ButtonComponent::LoadData(Define::EButtonState state, const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmaps[state] = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
	
	// ���� ������ �̹������ m_bitmap�� ������Ʈ
	if (state == m_state)
	{
		m_bitmap = m_bitmaps[state];
	}
}

bool ButtonComponent::IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize)
{
	return (mousePos.x >= uiPos.x &&
			mousePos.x <= uiPos.x + uiSize.x &&
			mousePos.y >= uiPos.y &&
			mousePos.y <= uiPos.y + uiSize.y);
}

void ButtonComponent::ExecuteStateAction(Define::EButtonState state)
{
	auto it = stateActionSlots.find(state);
	if (it != stateActionSlots.end())
	{
		// �θ� ��ü�� ��ȿ���� Ȯ��
		if (!it->second.weakPtr.expired())
		{
			it->second.func(); // �ش� ������ �Լ� ����
		}
		else
		{
			// �θ� ��ü�� �ı��Ǿ����� ���� ����
			stateActionSlots.erase(it);
		}
	}
}

void ButtonComponent::SetCurrentState(Define::EButtonState state)
{
	// ���� ���¿� �ٸ��� ���� ����
	if (m_state != state)
	{
		// ���� ������ ���� �Լ� ���� (�ִ� ���)
		ExecuteStateAction(static_cast<Define::EButtonState>(static_cast<int>(m_state) + 100)); // ���� ���´� +100���� ����
		
		m_state = state;
		
		// �� ������ ���� �Լ� ����
		ExecuteStateAction(state);
		
		// ���� ���¿� �´� �̹����� m_bitmap ������Ʈ
		auto it = m_bitmaps.find(state);
		if (it != m_bitmaps.end() && it->second)
		{
			m_bitmap = it->second;
		}
	}
}
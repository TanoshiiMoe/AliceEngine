#include "pch.h"
#include "UIButton.h"
#include <Helpers/FileHelper.h>
#include <Manager/PackageResourceManager.h>
#include <Core/Input.h>

UIButton::UIButton()
{
}

UIButton::~UIButton()
{
}

void UIButton::LoadData(const std::wstring& path)
{
    filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
    m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
        (Define::BASE_RESOURCE_PATH + path).c_str());
}

void UIButton::SetPosition(const FVector2& pos)
{
    m_transform.SetPosition(pos.x, pos.y);
}

void UIButton::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);

    FVector2 mousePos = Input::GetMousePosition();

    FVector2 ownerPos = FVector2(m_transform.GetPosition().x, m_transform.GetPosition().y);
    float width = GetScale().x;
    float height = GetScale().y;

    bool isInside =
        mousePos.x >= ownerPos.x &&
        mousePos.x <= ownerPos.x + width &&
        mousePos.y >= ownerPos.y &&
        mousePos.y <= ownerPos.y + height;

    bool mouseDown = Input::IsMouseLeftDown();
    bool mouseUp = Input::IsMouseLeftReleased();

    if (isInside)
    {
        SetCursor(LoadCursorW(nullptr, IDC_HAND));

        if (mouseDown)
        {
            m_state = ButtonState::Pressed;
            if (!m_pressedImage.empty())
                LoadData(m_pressedImage);
        }
        else
        {
            m_state = ButtonState::Hover;
            if (!m_hoverImage.empty())
                LoadData(m_hoverImage);
        }

        if (m_state == ButtonState::Pressed && m_prevMouseDown)
        {
            // 클릭 완료 판정 (눌렀다가 손 뗀 상태)
            if (onClick) onClick();
        }
    }
    else
    {
        m_state = ButtonState::Idle;
        if (!m_idleImage.empty())
            LoadData(m_idleImage);

        SetCursor(LoadCursorW(nullptr, IDC_ARROW));
    }

    // 이전 프레임 상태 저장
    m_prevMouseDown = mouseDown;
}

// Mouse State
// IDC_ARROW			Idle(Arrow)
// IDC_APPSTARTING		Arrow + Loading
// IDC_CROSS 			Cross
// IDC_HAND 			toClick
// IDC_HELP 			Arrow + ?
// IDC_IBEAM 			Text Input
// IDC_ICON				Invisibility
// IDC_NO 				Ban
// IDC_PERSON			Arrow + Person Mark
// IDC_PIN				Arrow + Pin Mark
// IDC_SIZEALL			SizeControl All
// IDC_SIZENESW			 
// IDC_SIZENS			
// IDC_SIZENWSE			
// IDC_SIZEWE			
// IDC_UPARROW
// IDC_WAIT				Loading
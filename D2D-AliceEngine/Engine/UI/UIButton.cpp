#include "pch.h"
#include "UIButton.h"
#include <Helpers/FileHelper.h>
#include <Manager/PackageResourceManager.h>
#include <Core/Input.h>
#include <Component/Component.h>
#include <Manager/SceneManager.h>
#include <Component/TransformComponent.h>

UIButton::UIButton()
{
}

UIButton::~UIButton()
{
}

void UIButton::SetImages(const std::wstring& idle, const std::wstring& hover, const std::wstring& pressed)
{
    m_idleImage = idle;
    m_hoverImage = hover;
    m_pressedImage = pressed;
    LoadData(m_idleImage); // 초기 이미지 세팅
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

void UIButton::SetPivot(float _x, float _y)
{
    owner->transform()->SetPivot(_x, _y);
}

void UIButton::SetAnchor(EUIScreenAnchor anchor, const FVector2& offset)
{
    m_anchor = anchor;

    FVector2 pos = FVector2(0, 0);
    SetPivot(0.5f, 0.5f);

    float screenWidth = SCREEN_WIDTH - GetScale().x;
    float screenHeight = SCREEN_HEIGHT - GetScale().y;

    switch (anchor)
    {
    case EUIScreenAnchor::TopLeft:
        break;

    case EUIScreenAnchor::TopCenter:
        pos = FVector2(screenWidth / 2.0f, 0);
        break;

    case EUIScreenAnchor::TopRight:
        pos = FVector2(screenWidth, 0);
        break;

    case EUIScreenAnchor::MiddleLeft:
        pos = FVector2(0, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::MiddleCenter:
        pos = FVector2(screenWidth / 2.0f, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::MiddleRight:
        pos = FVector2(screenWidth, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::BottomLeft:
        pos = FVector2(0, screenHeight);
        break;

    case EUIScreenAnchor::BottomCenter:
        pos = FVector2(screenWidth / 2.0f, screenHeight);
        break;

    case EUIScreenAnchor::BottomRight:
        pos = FVector2(screenWidth, screenHeight);
        break;

    default:
        break;
    }

    //Offset은 D2D좌표계를 사용합니다.
    SetPosition(pos + offset);
}

void UIButton::SetAnchor(EUIScreenAnchor anchor, const float& offsetX, const float& offsetY)
{
    m_anchor = anchor;

    FVector2 pos = FVector2(0, 0);
    SetPivot(0.5f, 0.5f);

    float screenWidth = SCREEN_WIDTH - GetScale().x;
    float screenHeight = SCREEN_HEIGHT - GetScale().y;

    switch (anchor)
    {
    case EUIScreenAnchor::TopLeft:
        break;

    case EUIScreenAnchor::TopCenter:
        pos = FVector2(screenWidth / 2.0f, 0);
        break;

    case EUIScreenAnchor::TopRight:
        pos = FVector2(screenWidth, 0);
        break;

    case EUIScreenAnchor::MiddleLeft:
        pos = FVector2(0, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::MiddleCenter:
        pos = FVector2(screenWidth / 2.0f, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::MiddleRight:
        pos = FVector2(screenWidth, screenHeight / 2.0f);
        break;

    case EUIScreenAnchor::BottomLeft:
        pos = FVector2(0, screenHeight);
        break;

    case EUIScreenAnchor::BottomCenter:
        pos = FVector2(screenWidth / 2.0f, screenHeight);
        break;

    case EUIScreenAnchor::BottomRight:
        pos = FVector2(screenWidth, screenHeight);
        break;

    default:
        break;
    }

    //Offset은 D2D좌표계를 사용합니다.
    SetPosition(pos + FVector2(offsetX, offsetY));
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
            m_state = EButtonState::Pressed;
            if (!m_pressedImage.empty())
                LoadData(m_pressedImage);
        }
        else
        {
            m_state = EButtonState::Hover;
            if (!m_hoverImage.empty())
            {
                LoadData(m_hoverImage);
            }
        }

        if (m_state == EButtonState::Pressed && m_prevMouseDown)
        {
            // 클릭 완료 판정 (눌렀다가 손 뗀 상태)
            if (onClick) onClick();
        }
    }
    else
    {
        m_state = EButtonState::Idle;
        if (!m_idleImage.empty())
        {
            LoadData(m_idleImage);
        }

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
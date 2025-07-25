#pragma once
#include <Component/RenderComponent.h>
#include <Manager/UIManager.h>

/*
*	UI ������Ʈ�Դϴ�.
*	�ؽ�Ʈ�� UIText(ī�޶� ����), ���� ���� Text�� �и�
*	���콺 Ŭ�� �̺�Ʈ ó���� ��ư������ ó���ϸ� �˴ϴ�.
*	�� �̹����� Ŭ���� �� �ְ� �Ѵٸ� ����ϼŵ� �Ǳ� �� ������...
*	�߰������� �̹����� �ִϸ��̼ǵ� ���� �� ������ ������Ʈ�� �����Ծ��
*/

class UIComponent : public RenderComponent
{
public:
	UIComponent();
	~UIComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	void Render() override;

	virtual float GetSizeX() override;
	virtual float GetSizeY() override;

	FVector2 GetSize();

	virtual void OnMouseEvent(const POINT& mousePos, bool isClick) {}

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};


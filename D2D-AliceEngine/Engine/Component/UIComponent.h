#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>

/*
*	UI ������Ʈ�Դϴ�.
*	�ؽ�Ʈ�� UIText(ī�޶� ����), ���� ���� Text�� �и�
*	���콺 Ŭ�� �̺�Ʈ ó���� ��ư������ ó���ϸ� �˴ϴ�.
*	�� �̹����� Ŭ���� �� �ְ� �Ѵٸ� ����ϼŵ� �Ǳ� �� ������...
*	�߰������� �̹����� �ִϸ��̼ǵ� ���� �� ������ ������Ʈ�� �����Ծ��
*/
class Canvas;
class TransformComponent;
struct ID2D1Bitmap1;
class UIComponent : public RenderComponent
{
public:
	UIComponent();
	~UIComponent();

	virtual void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	void SetScale(const FVector2& _scale) { scale = _scale; }
	void SetPosition(const FVector2& pos);
	FVector2 GetUIPivot();

	FVector2 GetSize();

	virtual void OnMouseEvent(const POINT& mousePos, bool isClick) {}

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	WeakObjectPtr<TransformComponent> GetUITransform() const;
	void AddChildUI(UIComponent* obj);

protected:
	WeakObjectPtr<UIComponent> m_parent = nullptr;
	std::vector<UIComponent*> m_child;

	WeakObjectPtr<TransformComponent> m_transformComponent;
};


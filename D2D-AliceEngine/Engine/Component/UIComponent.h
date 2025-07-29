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
	void AddChildUI(UIComponent* child);
	void SetParentUI(UIComponent* parent);

	std::vector<UIComponent*> m_components;

	//template<typename T, typename... Args>
	//T* AddUI(Args&&... args)
	//{
	//	T* newComp = new T(std::forward<Args>(args)...);
	//	newComp->SetParentUI(this);

	//	newComp->SetCanvas(this->GetCanvas());

	//	m_child.push_back(newComp);
	//	newComp->Initialize();
	//	return newComp;
	//}

	template<typename T>
	T* AddUI()
	{
		T* newUI = new T();

		AddChildUI(newUI);
		newUI->Initialize();

		return newUI;
	}

	template<class T>
	T* GetUI()
	{
		for (auto& comp : m_components)
		{
			// shared_ptr<Component> �� shared_ptr<T>�� ĳ����
			if (auto casted = dynamic_cast<T*>(comp))
			{
				return casted; // raw pointer ��ȯ
			}
		}
		return nullptr;
	}

	template<class T>
	bool RemoveUI(T* targetComponent)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it) == targetComponent)
			{
				(*it)->OnDestroy();
				m_components.erase(it);
				delete targetComponent;
				return true;
			}
		}
		return false;
	}

protected:
	WeakObjectPtr<UIComponent> m_parent = nullptr;
	std::vector<UIComponent*> m_child;

	WeakObjectPtr<TransformComponent> m_transformComponent;
};


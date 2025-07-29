#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>

/*
*	UI 컴포넌트입니다.
*	텍스트는 UIText(카메라에 고정), 고정 없는 Text로 분리
*	마우스 클릭 이벤트 처리는 버튼에서만 처리하면 됩니다.
*	뭐 이미지도 클릭할 수 있게 한다면 사용하셔도 되긴 함 ㅇㅅㅇ...
*	추가적으로 이미지가 애니메이션도 있을 수 있으니 업데이트도 가져왔어요
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

	std::wstring filePath; // 파일의 경로
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
			// shared_ptr<Component> → shared_ptr<T>로 캐스팅
			if (auto casted = dynamic_cast<T*>(comp))
			{
				return casted; // raw pointer 반환
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


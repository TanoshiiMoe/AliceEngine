#pragma once
#include "pch.h"
#include "Transform.h"

/*
* @brief Object 클래스.
* @details ComPtr로 ID2D1Bitmap1를 관리하고, Transform 컴포넌트를 포함합니다.
* @details 월드 변환을 재귀적으로 계산하고, 자식 객체의 Transform을 관리합니다.
*/

using namespace Microsoft::WRL;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object()
	{
		m_localTransform = std::make_shared<Transform>();
		m_worldTransform = std::make_shared<Transform>();
	}
	virtual ~Object()
	{
		m_localTransform.reset();
		m_worldTransform.reset();
		m_localTransform = nullptr;
		m_worldTransform = nullptr;
		//m_bitmap = nullptr;
	}

	std::shared_ptr<Transform> m_localTransform; // Transform 컴포넌트
	std::shared_ptr<Transform> m_worldTransform; // Transform 컴포넌트
	D2D1_VECTOR_2F m_pivot{0,0}; // 좌표 중심점
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage 컴포넌트

	virtual void Initialize();
	virtual void LoadBitmapData(const std::wstring& path);
	virtual void Release();
	virtual void Render();

	// 계층구조 관리
	std::weak_ptr<Object> parent;
	std::vector<std::shared_ptr<Object>> children;

	inline void AddChild(std::weak_ptr<Object> child)
	{
		if (child.lock() == nullptr) return; // nullptr 체크
		child.lock()->parent = this->shared_from_this();
		children.push_back(child.lock());
	}

	inline void SetPivot(const float& _x, const float& _y)
	{
		m_pivot.x = _x;
		m_pivot.y = _y;
	}
	inline void SetPivot(const float& _x)
	{
		m_pivot.x = _x;
		m_pivot.y = _x;
	}

	// 재귀로 월드 변환 계산
	void UpdateWorldTransform();
};


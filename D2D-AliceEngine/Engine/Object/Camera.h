#pragma once
#include "Object/UObject.h"
#include <Math/Transform.h>

/*
* @brief 카메라 클래스. 
* @details 현재는 Object를 상속받아 기본적인 기능만 구현되어 있습니다.
*/
class Camera : public UObject
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	inline void SetPosition(const float& _x, const float& _y)
	{
		m_transform->SetPosition(_x, _y);
	}

	inline FVector2 GetPosition()
	{
		return FVector2(m_transform->GetPosition().x, m_transform->GetPosition().y);
	}

	inline void SetRotation(const float& _val)
	{
		m_transform->SetRotation(_val);
	}

	inline void AddRotation(const float& _val)
	{
		m_transform->SetRotation(m_transform->GetRotation() + _val);
	}

	inline void AddPosition(const float& _x, const float& _y)
	{
		m_transform->SetPosition(m_transform->GetPosition().x + _x, m_transform->GetPosition().y + _y);
	}

	inline void SetOwner(gameObject* obj)
	{
		owner = obj;
	}
	inline void ClearOwner()
	{
		owner = nullptr;
	}
	gameObject* owner = nullptr;
	std::shared_ptr<Transform> m_transform;
};


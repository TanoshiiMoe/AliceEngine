#pragma once
#include "Object/UObject.h"

/*
* @brief 카메라 클래스. 
* @details 현재는 Object를 상속받아 기본적인 기능만 구현되어 있습니다.
*/
class Transform;
class gameObject;

class Camera : public UObject
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	FVector2 GetScale();
	void SetScale(const FVector2& scale);

	void SetPosition(const float& _x, const float& _y);
	FVector2 GetPosition();
	float GetPositionX();
	float GetPositionY();

	void SetRotation(const float& _val);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	void SetOwner(gameObject* obj);
	void ClearOwner();

	void SetFieldOfView(float& InFieldOfView) { fieldOfView = InFieldOfView; }
	float fieldOfView = 1.0f;
	gameObject* owner = nullptr;
	std::unique_ptr<Transform> m_transform;
};


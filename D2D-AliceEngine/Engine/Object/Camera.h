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

	void SetPosition(const float& _x, const float& _y);
	FVector2 GetPosition();

	void SetRotation(const float& _val);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	void SetOwner(gameObject* obj);
	void ClearOwner();
	gameObject* owner = nullptr;
	std::shared_ptr<Transform> m_transform;
};


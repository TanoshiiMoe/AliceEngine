#pragma once
#include "Object/UObject.h"

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
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

	D2D1_POINT_2F TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt);

	// ��ǥ ��ȯ �Լ���
	D2D1_POINT_2F ViewportToWorldPoint(const D2D1_POINT_2F& viewport);
	D2D1_POINT_2F WorldToViewportPoint(const D2D1_POINT_2F& world);

	D2D1_POINT_2F ScreenToWorldPoint(const D2D1_POINT_2F& screenPos);
	D2D1_POINT_2F WorldToScreenPoint(const D2D1_POINT_2F& world);

	void SetFieldOfView(float& InFieldOfView) { fieldOfView = InFieldOfView; }
	float fieldOfView = 1.0f;
	gameObject* owner = nullptr;
	std::unique_ptr<Transform> m_transform;
};


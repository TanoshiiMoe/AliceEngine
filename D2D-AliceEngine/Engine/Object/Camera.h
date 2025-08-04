#pragma once
#include "Object/UObject.h"
#include <Component/TransformComponent.h>

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

	// ��� ��ǥ, ������, ȸ�� ���� �Լ���
	void SetRelativePosition(const FVector2& _pos);
	void SetRelativeRotation(const float& _rotation);
	void SetRelativeScale(const FVector2& _scale);
	
	void AddRelativePosition(const FVector2& _pos);
	void AddRelativeRotation(const float& _rotation);
	void AddRelativeScale(const FVector2& _scale);
	
	FVector2 GetRelativePosition() const;
	float GetRelativeRotation();
	FVector2 GetRelativeScale();

	void SetOwner(gameObject* obj);
	void ClearOwner();

	// �θ�-�ڽ� ���� ���� �Լ���
	void AddChildObject(gameObject* obj);
	void RemoveFromParent();

	D2D1_POINT_2F TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt);

	// ��ǥ ��ȯ �Լ���
	D2D1_POINT_2F ViewportToWorldPoint(const D2D1_POINT_2F& viewport);
	D2D1_POINT_2F WorldToViewportPoint(const D2D1_POINT_2F& world);

	D2D1_POINT_2F ScreenToWorldPoint(const D2D1_POINT_2F& screenPos);
	D2D1_POINT_2F WorldToScreenPoint(const D2D1_POINT_2F& world);

	// D2D ��ũ�� �ȼ� �� UI ��ǥ�� (0~1, ���ϴ� ����)
	D2D1_POINT_2F ScreenToUICoord(const D2D1_POINT_2F& screen);
	// UI ��ǥ�� (0~1, ���ϴ� ����) �� D2D ��ũ�� �ȼ�
	D2D1_POINT_2F UICoordToScreen(const D2D1_POINT_2F& ui);

	void SetFieldOfView(float& InFieldOfView) { fieldOfView = InFieldOfView; }
	float fieldOfView = 1.0f;
	gameObject* owner = nullptr;
	TransformComponent relativeTransform;
};


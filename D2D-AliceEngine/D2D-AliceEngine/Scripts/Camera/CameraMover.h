#pragma once
#include <Component/ScriptComponent.h>
#include <Object/gameObject.h>
#include "Object/Camera.h"

class BikeMovementScript;
class SkewTransform;
class CameraMover : public ScriptComponent
{
public:
	void Initialize() override;
	void Awake() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;
private:
	WeakObjectPtr<SkewTransform> playerST;
	Camera* camera;

	float xPos{0};
	float yPos{0};

	// ī�޶� �̵� �ӵ� ����
	float lerpSpeed = 5.0f;
	// Dead zone (ī�޶� ���ο��� �÷��̾ �����Ӱ� ������ �� �ִ� ����)
	float deadZoneX = 50.0f;
	float deadZoneY = 30.0f;
};


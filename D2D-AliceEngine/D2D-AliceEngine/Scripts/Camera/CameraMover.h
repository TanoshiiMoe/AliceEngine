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

	// 카메라 이동 속도 제어
	float lerpSpeed = 5.0f;
	// Dead zone (카메라 내부에서 플레이어가 자유롭게 움직일 수 있는 영역)
	float deadZoneX = 50.0f;
	float deadZoneY = 30.0f;
};


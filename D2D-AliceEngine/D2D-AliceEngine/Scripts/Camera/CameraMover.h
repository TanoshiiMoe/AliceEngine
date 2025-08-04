#pragma once
#include <Component/ScriptComponent.h>
#include <Object/gameObject.h>
#include "Object/Camera.h"

class BikeMovementScript;
class CameraMover : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void SetPlayer(gameObject* _player);
private:
	WeakObjectPtr<gameObject> player;
	BikeMovementScript* playerBM;
	Camera* camera;

	float xPos;
	float yPos;
};


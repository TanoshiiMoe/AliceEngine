#pragma once

#include "Component/ScriptComponent.h"
#include "Component/TransformComponent.h"
#include "Object/gameObject.h"

class SkewTransform : public ScriptComponent
{
public:
	WeakObjectPtr<gameObject> groundTile;
	//TransformComponent transform;

	float zPos = 0.0f;
	
	void SetClamp(float minZ = 0.0f, float maxZ = 0.0f);
	const FVector2 GetOffset();
	const FVector2 GetRealPos() { return realPos; }

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;
private:
	TransformComponent* renderTransform = nullptr;
	std::pair<float, float> clamp = { 0.0f, 0.0f };

	FVector2 offset = { 0.0f, 0.0f };
	FVector2 realPos = { 0.0f, 0.0f };
	float skewDeg = 0.0f;
	float prevzPos = -21359786.0f;

	float GetSkew();
};


#pragma once

#include "Component/ScriptComponent.h"
#include "Component/Animator.h"

class Prism : public ScriptComponent
{
private:
	bool isActive = true;
	bool isEnabled = true;
	bool isDirty = false;

	int prismCount = 0;
	float interval = 0.0f;
	float ellipsedTime = 0.0f;
	
	//std::deque<ID2D1Bitmap1> bitmaps;
	std::deque<WeakObjectPtr<gameObject>> objects;

	// 애니메이터 관련
	Animator* animator = nullptr;
	std::weak_ptr<ID2D1Bitmap1> bitmap;
	SpriteInfo* spriteInfo = nullptr;

public:
	Prism(int _prismCount = 0, float _interval = 0.0f) : prismCount(_prismCount), interval(_interval) {};
	~Prism();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	void GetSpriteInfo();
	void GetCurrBitmap();
	void MakeEffect();
	void ChangeColor();
	void DeleteLast();
	void ClearObjects();

	void SetActive(bool _val);
	bool IsActive();
};


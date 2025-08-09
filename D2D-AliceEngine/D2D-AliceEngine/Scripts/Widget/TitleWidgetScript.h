#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class SpriteRenderer;
class TitleWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;

private:
	//void WidgetClose();
    SpriteRenderer* m_bgmControl = nullptr;
    SpriteRenderer* m_sfxControl = nullptr;
    float m_bgmLeftAnchorX = 0.0f;
    float m_sfxLeftAnchorX = 0.0f;
};
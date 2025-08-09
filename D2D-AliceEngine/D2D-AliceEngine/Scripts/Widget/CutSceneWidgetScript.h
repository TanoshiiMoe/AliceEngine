#pragma once
#include <Component/ScriptComponent.h>
#include <vector>

class gameObject;
class SpriteRenderer;
class ButtonComponent;
class TextRenderComponent;
class AudioComponent;

class CutSceneWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

private:
	gameObject* m_owner;
	SpriteRenderer* m_background;
	ButtonComponent* m_nextButton;
	ButtonComponent* m_prevButton;
	ButtonComponent* m_skipButton;
	TextRenderComponent* m_nextText;
	TextRenderComponent* m_prevText;
	TextRenderComponent* m_skipText;
	AudioComponent* m_uiSound;

	// 컷씬 관련
	std::vector<std::wstring> m_cutSceneImages;
	int m_currentImageIndex;

	void LoadCutSceneImages();
	void ShowImage(int index);
	void NextImage();
	void PrevImage();
	void SkipCutScene();
};

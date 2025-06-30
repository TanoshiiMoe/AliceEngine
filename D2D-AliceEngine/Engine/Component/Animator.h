#pragma once
#include <Component/RenderComponent.h>

struct SpriteRenderer;
struct AnimationClip;
struct SpriteSheet;

class Animator : public RenderComponent
{
public:
	Animator();
	~Animator();

	virtual void Initialize() override;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override;

	virtual void Render() override;

	void SetAnimationClip(AnimationClip* clip);
	void PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip);
	void LoadSpriteData(const std::string& filePath);
	static void LoadSpriteSheet(const std::string& filePath, std::weak_ptr<SpriteSheet> spriteSheet);
	static void LoadAnimationClip(const std::string& filePath, std::weak_ptr<AnimationClip> clip, const std::weak_ptr<SpriteSheet> spriteSheet);

	void LoadSpriteRenderer(std::weak_ptr<SpriteSheet> sheet);

	void Play();
	void Stop();

	float time = -1;
	AnimationClip* pAnimationClip;
	std::weak_ptr<SpriteSheet> sheet;
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	size_t m_curClip = 0;
	bool bPlay = false;
	float m_fFPSTime = 1 / 12.0f;
	float m_fFPSLastTime = 0;
};
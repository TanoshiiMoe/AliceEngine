#pragma once
#include <Component/RenderComponent.h>

/*
* @briefs :
*	Sprite Animation을 실행하기 위한 Animator입니다.
*	비트맵을 json에 명세되어 있는 크기로 잘라서 그립니다.
* @details :

	사용법은 다음과 같습니다. 
	1. SpriteSheet와 AnimationClip 오브젝트를 생성합니다. 
	(예시에서는 std::shared_ptr을 사용했지만 관리를 잘할 자신이 있다면 rawPointer로도 가능합니다.)
Texture = std::make_shared<SpriteSheet>();
idle = std::make_shared<AnimationClip>();
  
	2. Animator의 static 함수로 해당하는 경로에서 json 파일을 읽어옵니다.
	(경로는 Resource/ 의 하위 디렉토리)
Animator::LoadSpriteSheet("ken_sprites.json", Texture);
Animator::LoadAnimationClip("ken_idle_anim.json", idle, Texture);

	3. Animator 컴포넌트를 추가하고 클립을 설정한 다음 Texture를 Load해주고 그 이후 기능을 사용합니다.
m_animator = m_owner->AddComponent<Animator>();
m_animator->SetAnimationClip(idle.get());
m_animator->LoadSpriteRenderer(Texture);
	4. 기능
m_animator->SetLooping(true);
m_animator->Play();

* 
*/

struct SpriteRenderer;
struct AnimationClip;
struct SpriteSheet;
class ID2D1Bitmap1;
class Animator : public RenderComponent
{
public:
	Animator();
	~Animator();

	virtual void Initialize() override;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override;
	virtual void Render() override;

	// 애니메이션 사전 준비 함수
	void SetAnimationClip(AnimationClip* clip);
	void PlayAnimation(std::weak_ptr<SpriteSheet> sheet, std::weak_ptr<AnimationClip> clip);
	void LoadSpriteData(const std::string& filePath);
	static void LoadSpriteSheet(const std::string& filePath, std::weak_ptr<SpriteSheet> spriteSheet);
	static void LoadAnimationClip(const std::string& filePath, std::weak_ptr<AnimationClip> clip, const std::weak_ptr<SpriteSheet> spriteSheet);
	void LoadSpriteRenderer(std::weak_ptr<SpriteSheet> sheet);

	// 애니메이션 실행, 중지, 변경 등의 함수
	void Play();
	void Stop();
	void PlayAtFrame(const size_t& _frame);
	void PlayAtStart();
	void StopAndResetFrame();
	bool IsEnd();
	void SetLooping(const bool& loop) { bLoopping = loop; }
	bool IsLooping() { return bLoopping; }
	void ChangeAnimation(const std::string& _key, bool _loop = true);
	void SetFlip(const bool& _flip) { bFlip = _flip; }

	float time = -1;
	AnimationClip* pAnimationClip;
	std::weak_ptr<SpriteSheet> sheet;
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	size_t m_curTime = 0;
	size_t m_curFrame = 0;
	bool bFlip = false;
	bool bPlay = false;
	bool bLoopping = false;
	float m_accumTime = 0;
};
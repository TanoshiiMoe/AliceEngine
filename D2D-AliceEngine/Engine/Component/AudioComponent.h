#pragma once
#include <Component/Component.h>

/*
*	오디오 컴포넌트 입니다.
*	fmod 적용, dll은 일단 추후 추가 예정
*/

class AudioComponent : public Component
{
public:
	AudioComponent();
	~AudioComponent();

	void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;

};


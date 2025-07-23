#pragma once
#include <Component/Component.h>
#include <Manager/AudioManager.h>

/*
*	����� ������Ʈ �Դϴ�.
*	fmod ����, dll�� �ϴ� ���� �߰� ����
*/

class AudioComponent : public Component
{
public:
	AudioComponent();
	~AudioComponent();

	void Initialize() override;

	void Load(
		const char* audioPath,
		AudioMode audioMode);

	void Play(
		float volume = 1.0f,
		bool paused = false);

	void SetVolume(float volume);

	bool IsPlaying() const;

	void Pause(bool paused = true);

	void Resume();

	void Stop();

	float GetPlayTime() const;

private:
	FMOD::Sound* m_Sound = nullptr;
	FMOD::Channel* m_Channel = nullptr;
	bool isLoaded = false;
};


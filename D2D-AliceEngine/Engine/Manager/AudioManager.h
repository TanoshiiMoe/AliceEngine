#pragma once
#include <Core/Singleton.h>
#include <fmod.hpp>
#pragma comment(lib, "fmod_vc.lib")

enum class AudioMode : uint32_t
{
	Loop = FMOD_LOOP_NORMAL | FMOD_2D,
	Default = FMOD_DEFAULT
};

class AudioManager : public Singleton<AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	// ���� ������Ʈ�� �����ϰ� �ε嵥���͸� ���ؼ� ȣ��
	void LoadSound(const std::wstring& path, AudioMode audioMode,FMOD::Sound** sound);

	void PlaySound(
		FMOD::Sound* sound,
		FMOD::Channel** channel,
		float volume = 1.0f,
		bool paused = false);	// true : �Ͻ�����


	void PauseSound(
		FMOD::Channel* channel,
		bool paused = true
	);

	void ResumeSound(FMOD::Channel* channel);

	void StopSound(FMOD::Channel* channel);

	void Uninitialize();

	float GetMusicTime(FMOD::Channel* channel);
	
private:

	bool isLoaded = false;
};


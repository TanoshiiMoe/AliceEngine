#pragma once
#include <Core/Singleton.h>
#include <Helpers/FFmpegHelper.h>
#include <fmod.hpp>
#pragma comment(lib, "fmod_vc.lib")

enum class AudioMode : uint32_t
{
	Memory = FMOD_DEFAULT | FMOD_2D,							// ª�� ȿ����
	MemoryLoop = FMOD_DEFAULT | FMOD_LOOP_NORMAL | FMOD_2D,     // ª�� ȿ���� �ݺ�
	Stream = FMOD_CREATESTREAM | FMOD_2D,						// �� �����
	StreamLoop = FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_2D // �� ����� �ݺ�
};

class AudioManager : public Singleton<AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	// ���� ���� ��� ����
	std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;

	void Initialize();
	void Update();

	// ���� ������Ʈ�� �����ϰ� Extension���� ���� �����ؼ� dll �ű� ����
	void LoadSound(const char* path, AudioMode audioMode,FMOD::Sound** sound);

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

	float GetMusicTime(FMOD::Channel* channel);
	
	std::wstring fileDirPath; // ����� ���� ���

private:
	FMOD::System* m_System = nullptr;
};


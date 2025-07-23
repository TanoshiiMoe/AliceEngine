#pragma once
#include <Core/Singleton.h>
#include <Helpers/FFmpegHelper.h>
#include <fmod.hpp>
#pragma comment(lib, "fmod_vc.lib")

enum class AudioMode : uint32_t
{
	Memory = FMOD_DEFAULT | FMOD_2D,							// 짧은 효과음
	MemoryLoop = FMOD_DEFAULT | FMOD_LOOP_NORMAL | FMOD_2D,     // 짧은 효과음 반복
	Stream = FMOD_CREATESTREAM | FMOD_2D,						// 긴 배경음
	StreamLoop = FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_2D // 긴 배경음 반복
};

class AudioManager : public Singleton<AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	// 사운드 보관 방식 변경
	std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;

	void Initialize();
	void Update();

	// 비디오 컴포넌트랑 동일하게 Extension에서 파일 복사해서 dll 옮길 예정
	void LoadSound(const char* path, AudioMode audioMode,FMOD::Sound** sound);

	void PlaySound(
		FMOD::Sound* sound,
		FMOD::Channel** channel,
		float volume = 1.0f,
		bool paused = false);	// true : 일시정지

	void PauseSound(
		FMOD::Channel* channel,
		bool paused = true
	);

	void ResumeSound(FMOD::Channel* channel);

	void StopSound(FMOD::Channel* channel);

	float GetMusicTime(FMOD::Channel* channel);
	
	std::wstring fileDirPath; // 오디오 파일 경로

private:
	FMOD::System* m_System = nullptr;
};


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

enum class SoundType { BGM, SFX };

struct SoundData
{
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	AudioMode mode;
	std::wstring path;
	SoundType type;
};

class AudioManager : public Singleton<AudioManager>
{
public:
	AudioManager();
	~AudioManager();

	// ���� ����
	std::unordered_map<std::wstring, SoundData*> m_SoundMap;

	void Initialize();
	void Update();

	void LoadSound(
		const std::wstring& name,
		const std::wstring& path,
		AudioMode audioMode,
		SoundType type,
		SoundData** sound);

	void PlaySound(
		FMOD::Sound* sound,
		FMOD::Channel** channel,
		float volume = 1.0f,
		float startTime = 0,
		bool paused = false
	);	// true : �Ͻ�����

	void PlaySoundByName(
		const std::wstring& name,
		FMOD::Channel** outChannel,
		float volume,
		float startTime,
		bool paused);
	void PauseSoundByName(const std::wstring& name, bool paused);
	void ResumeSoundByName(const std::wstring& name);
	void StopSoundByName(const std::wstring& name);

	void PlaySoundByType(
		SoundType type,
		FMOD::Channel** outChannel,
		float volume,
		float startTime,
		bool paused);
	void PauseSoundByType(SoundType type, bool paused);
	void ResumeSoundByType(SoundType type);
	void StopSoundByType(SoundType type);

	void PauseAll();
	void ResumeAll();
	void StopAll();

	float GetMusicTime(FMOD::Channel* channel);

	// Getter | Setter
	float GetMasterVolume();
	void SetMasterVolume(float volume);

	float GetBGMVolume() const;
	void SetBGMVolume(float volume);

	float GetSFXVolume() const;
	void SetSFXVolume(float volume);

	//float GetUIVolume() const;
	//void SetUIVolume(float volume);
	
	std::wstring fileDirPath; // ����� ���� ���

private:
	FMOD::System* m_System = nullptr;
	FMOD::ChannelGroup* m_MasterGroup = nullptr;
	FMOD::ChannelGroup* m_BGMGroup = nullptr;
	FMOD::ChannelGroup* m_SFXGroup = nullptr;
	//FMOD::ChannelGroup* m_UIGroup = nullptr;

	float m_masterVolume;
	float m_bgmVolume = 0.5f;
	float m_sfxVolume = 0.5f;
	//float m_UIVolume = 0.3f;
	// �ʿ�� �߰�
};


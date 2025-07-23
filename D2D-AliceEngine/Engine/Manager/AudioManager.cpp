#include "pch.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	for (auto& pair : m_SoundMap)
	{
		if (pair.second)
		{
			pair.second->release();
		}
	}

	m_SoundMap.clear();

	if (m_System)
	{
		m_System->close();
		m_System->release();
		m_System = nullptr;
	}
}

void AudioManager::Initialize()
{
	if (m_System) return;

	FMOD_RESULT fr = FMOD::System_Create(&m_System);
	if (fr != FMOD_OK) return;

	fr = m_System->init(512, FMOD_INIT_NORMAL, nullptr);
	if (fr != FMOD_OK) return;
}

void AudioManager::Update()
{
	m_System->update();
}

void AudioManager::LoadSound(const char* path, AudioMode audioMode, FMOD::Sound** sound)
{
	auto it = m_SoundMap.find(path);
	if (it != m_SoundMap.end())
	{
		*sound = it->second;
		return;
	}

	FMOD::Sound* newSound = nullptr;

	FMOD_RESULT fr = m_System->createSound(path, static_cast<FMOD_MODE>(audioMode), nullptr, &newSound);
	if (fr != FMOD_OK)
	{
		return;
	}

	m_SoundMap[path] = newSound;

	*sound = newSound;
}

void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel, float volume, bool paused)
{
	if (!sound || !channel) return;

	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	FMOD_RESULT fr = m_System->playSound(sound, nullptr, paused, channel);

	if (fr != FMOD_OK) return;

	if (*channel) (*channel)->setVolume(volume);
}

void AudioManager::PauseSound(FMOD::Channel* channel, bool paused)
{
	if (channel) channel->setPaused(paused);
}

void AudioManager::ResumeSound(FMOD::Channel* channel)
{
	PauseSound(channel, false);
}

void AudioManager::StopSound(FMOD::Channel* channel)
{
	if (channel) channel->stop();
}

float AudioManager::GetMusicTime(FMOD::Channel* channel)
{
	if (!channel) return 0.0f;

	unsigned int time = 0;
	FMOD_RESULT result = channel->getPosition(&time, FMOD_TIMEUNIT_MS);

	if (result == FMOD_OK)
	{
		return static_cast<float>(time) / 1000.0f;	// 밀리초 단위 -> 초 단위
	}

	return 0.0f;
}

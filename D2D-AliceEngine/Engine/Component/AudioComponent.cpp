#include "pch.h"
#include "AudioComponent.h"

AudioComponent::AudioComponent()
{
}

AudioComponent::~AudioComponent()
{
	//Stop();
}

void AudioComponent::Initialize()
{
	AudioManager::GetInstance().Initialize();

	m_Channel = nullptr;
}

void AudioComponent::Load(const std::wstring& audioPath, AudioMode audioMode)
{
	AudioManager::GetInstance().LoadSound(audioPath, audioMode, &m_Sound);
}

void AudioComponent::Play(float sec, float volume, bool paused)
{
	int startSec = sec * 1000;	// s -> ms

	if (!m_Sound) return;

	AudioManager::GetInstance().PlaySound(m_Sound, &m_Channel, volume, startSec, paused);
}

void AudioComponent::SetVolume(float volume)
{
	if (m_Channel)
		m_Channel->setVolume(volume);
}

bool AudioComponent::IsPlaying() const
{
	bool isPlaying = false;

	if (m_Channel)
		m_Channel->isPlaying(&isPlaying);

	return isPlaying;
}

void AudioComponent::Pause(bool paused)
{
	if (m_Channel)
		AudioManager::GetInstance().PauseSound(m_Channel, paused);
}

void AudioComponent::Resume()
{
	Pause(false);
}

void AudioComponent::Stop()
{
	if (m_Channel)
		AudioManager::GetInstance().StopSound(m_Channel);
}

float AudioComponent::GetPlayTime() const
{
	return AudioManager::GetInstance().GetMusicTime(m_Channel);
}
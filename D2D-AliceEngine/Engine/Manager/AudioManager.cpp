#include "pch.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::Initialize()
{
}

void AudioManager::Update()
{
}

void AudioManager::LoadSound(const std::wstring& path, AudioMode audioMode, FMOD::Sound** sound)
{
}

void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel, float volume, bool paused)
{
}

void AudioManager::PauseSound(FMOD::Channel* channel, bool paused)
{
}

void AudioManager::ResumeSound(FMOD::Channel* channel)
{
}

void AudioManager::StopSound(FMOD::Channel* channel)
{
}

void AudioManager::Uninitialize()
{
}

float AudioManager::GetMusicTime(FMOD::Channel* channel)
{
	return 0.0f;
}

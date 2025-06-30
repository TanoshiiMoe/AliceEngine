#pragma once
#include <Helpers/TextureLoader.h>
#include <unordered_map>

enum class EState
{
	Idle,
	Walk,
	Run,
	Attack,
	Max
};

class AnimationController
{
	AnimationController() {}
	~AnimationController() {}

	std::unordered_map<EState, std::weak_ptr<AnimationClip>> Clips;

	void Transition();
};


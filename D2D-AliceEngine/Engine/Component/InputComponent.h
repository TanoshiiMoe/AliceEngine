#pragma once
#include "Component.h"
#include "System/InputSystem.h"
#include <memory>
#include <vector>
#include <functional>
/*
* @briefs : 인풋 컴포넌트입니다.
* @details : actions에 함수 포인터를 등록하면 Update()시에 해당 함수가 실행됩니다.
*/

class InputComponent : public Component
{
public:
	InputComponent();
	~InputComponent();
public:
	virtual void Initialize();
	void Update(const float& deltaSeconds) override;
	virtual void Release();
	
	template<typename F>
	void SetAction(F&& action)
	{
		actions.emplace_back(std::forward<F>(action));
	}

private:
	std::vector<std::function<void()>> actions;
};
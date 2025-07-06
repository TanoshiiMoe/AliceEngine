#pragma once
#include <functional>
#include <Core/ObjectHandler.h>
class Delegate
{
};

template<typename... Args>
class MultiDelegate 
{
	struct Slot 
	{
		ObjectHandle handle; // 콜백 구분용 포인터(주로 this)
		std::function<void(Args...)> func;
	};
	std::vector<Slot> slots;

public:
	// Handle을 같이 받는다.
	void Add(ObjectHandle handle, const std::function<void(Args...)>& f) 
	{
		slots.push_back({ handle, f });
	}
	// tag(포인터)로 삭제
	void Remove(ObjectHandle handle) 
	{
		slots.erase(
			std::remove_if(slots.begin(), slots.end(),
				[handle](const Slot& s) { return s.handle == handle; }),
			slots.end());
	}
	void Clear() { slots.clear(); }
	void BroadCast(Args... args) const 
	{
		for (const auto& s : slots)
		{
			// Handle로 테이블에서 유효한지 검사
			if (ObjectHandler::GetInstance().IsValid(s.handle))
				s.func(args...);
		}

	}
};


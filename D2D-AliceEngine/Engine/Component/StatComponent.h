#pragma once
#include "Core/Delegate.h"
#include <unordered_map>
#include <typeindex>

class StatComponent
{
public:
	float HP = 100;
	float MP = 100;

	template<typename T>
	void RegisterEffectHandler(std::function<void(StatComponent*, const T&)> handler) {
		// type_index로 타입별 저장해보기 
		handlers[typeid(T)] = [handler](StatComponent* stat, const void* effect) {
			handler(stat, *static_cast<const T*>(effect));
			};
	}

	template<typename T>
	void ApplyEffect(const T& effect) {
		auto it = handlers.find(typeid(T));
		if (it != handlers.end()) {
			it->second(this, &effect);
		}
	}

private:
	// 인스턴스별로 타입마다 핸들러 저장해보기 
	std::unordered_map<std::type_index, std::function<void(StatComponent*, const void*)>> handlers;
};
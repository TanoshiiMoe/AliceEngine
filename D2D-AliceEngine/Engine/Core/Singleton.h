#pragma once
#include <cassert>

/*
* @brief Singleton Ŭ����.
* @details �� Ŭ������ ��ӹ��� Ŭ������ �̱��� ������ ������ �� �ֽ��ϴ�.
*/

template <typename T>
class Singleton {
public:
	Singleton()
	{
		assert(s_instance == nullptr && "Singleton instance already created!");
		s_instance = static_cast<T*>(this);
	}

	virtual ~Singleton() = default;

	static void Create()
	{
		if (!s_instance)
			s_instance = new T();
	}
	// ������� �ν��Ͻ� �ı�
	static void Destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	// ���� �� �̵� ����
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	static T& GetInstance()
	{
		assert(s_instance != nullptr && "Singleton instance not created!");
		return *s_instance;
	}
private:
	static T* s_instance;
};

template <typename T>
T* Singleton<T>::s_instance = nullptr;

#define GetSingleton(type) Singleton<type>::Get()
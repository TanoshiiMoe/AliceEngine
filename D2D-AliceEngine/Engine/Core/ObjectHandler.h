#pragma once
#include <assert.h>
#include <vector>
#include <queue>
#include <cstdint>
#include <unordered_set>
#include <core/Singleton.h>

class UObject;
class ObjectHandler : public Singleton<ObjectHandler>
{
	//ObjectTable의 생성자 / 소멸자를 private으로 선언했기 때문에, 
// 일반 코드는 new MyManager()나 delete를 직접 호출할 수 없습니다.
	friend class Singleton<ObjectHandler>;// ← 없으면 아래 Create()에서 컴파일 에러
private:
	ObjectHandler() = default;
	~ObjectHandler() = default;

	std::unordered_set<UObject*> m_objects;      // 생성된 인스턴스 주소 저장
public:

	// 인스턴스 주소가 존재하는지 검사
	bool IsValid(UObject* instance) {
		return m_objects.find(instance) != m_objects.end();
	}
	void Register(UObject* instance) {
		m_objects.insert(instance);
	}
	void Unregister(UObject* instance) {
		m_objects.erase(instance);
	}
};
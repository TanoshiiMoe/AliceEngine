#pragma once
#include <vector>
#include <queue>
#include <cstdint>
#include <cassert>
#include "Singleton.h"

class UObject;

struct ObjectHandle 
{
    uint32_t index = 0;
    uint32_t generation = 0;

    bool operator==(const ObjectHandle& other) const 
    {
        return index == other.index && generation == other.generation;
    }
};

class ObjectHandler : public Singleton<ObjectHandler>
{
    friend class Singleton<ObjectHandler>;
private:
    ObjectHandler() = default;
    ~ObjectHandler() = default;

    std::vector<UObject*> objects;      // 인스턴스 포인터 저장
    std::vector<uint32_t> generations; // 인덱스별 generation
    std::queue<uint32_t> freeIndexes;  // 파괴된 인덱스 재사용

public:
    // 핸들로 오브젝트 포인터 반환
    UObject* GetObjectPtr(const ObjectHandle& h) 
    {
        if (h.index >= objects.size() || !objects[h.index]) return nullptr;
        if (generations[h.index] != h.generation) return nullptr;
        return objects[h.index];
    }

    // 타입 안전 다운캐스팅
    template<typename T>
    T* GetTypePtr(const ObjectHandle& h) 
    {
        UObject* base = GetObjectPtr(h);
        return dynamic_cast<T*>(base);
    }

    // 핸들 생성 (새 오브젝트 등록)
    ObjectHandle CreateHandle(UObject* allocated)
    {
        uint32_t index = 0;
        if (freeIndexes.empty()) {
            index = static_cast<uint32_t>(objects.size());
            generations.push_back(0);
            objects.push_back(allocated);
        }
        else {
            index = freeIndexes.front();
            freeIndexes.pop();
            generations[index]++;
            assert(objects[index] == nullptr);
            objects[index] = allocated;
        }
        return ObjectHandle{ index, generations[index] };
    }

    // 핸들 파괴 (오브젝트 제거)
    void DestroyHandle(const ObjectHandle& h) {
        if (IsValid(h)) {
            objects[h.index] = nullptr;
            freeIndexes.push(h.index);
            // generation은 다음 생성에서 증가
        }
    }

    // 핸들 유효성 검사
    bool IsValid(const ObjectHandle& h) const {
        return h.index < objects.size()
            && objects[h.index] != nullptr
            && generations[h.index] == h.generation;
    }

    // 안전한 오브젝트 포인터 반환
    UObject* Get(const ObjectHandle& h) {
        return IsValid(h) ? objects[h.index] : nullptr;
    }
};

template<typename T>
class WeakObjectPtr {
public:
    T* ptr = nullptr;
    ObjectHandle handle;

    WeakObjectPtr() = default;
    WeakObjectPtr(T* instance)
    {
        ptr = instance;
        // UObject*에서 Handle을 관리할거기 때문에 이렇게 캐스팅
        if (auto uobj = dynamic_cast<UObject*>(instance))
            handle = uobj->GetHandle();
        else
            handle = ObjectHandle{}; // invalid handle
    }

    bool IsValid() const {
        return ptr != nullptr && ObjectHandler::GetInstance().IsValid(handle);
    }

    T* lock() const {
        return IsValid() ? ptr : nullptr;
    }

    T* Get() const {
        return lock();
    }

    T* operator->() const {
        return lock(); // 반드시 유효성 체크 후 사용
    }

    bool expired() const {
        return !IsValid();
    }

    void reset() {
        ptr = nullptr;
        handle = ObjectHandle{};
    }

    explicit operator bool() const {
        return IsValid();
    }
};

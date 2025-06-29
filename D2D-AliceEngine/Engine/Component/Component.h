#pragma once

#include <string>
#include <memory>
#include <Math/TMath.h>
#include <Core/Tickable.h>

class gameObject;
class Transform;
class Component : public ITickable, public std::enable_shared_from_this<Component>
{
public:
	Component();
	Component(const std::wstring& name);
	virtual ~Component() {};
public:
	virtual void Initialize() = 0;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() = 0;

	// Render가 필요없는 Component도 있음
	virtual void Render() {};

	virtual void Awake() {};
	virtual void OnStart() {}
	virtual void OnEnd() {}
	virtual void OnCreate() {}
	virtual void OnDestroy() {}

public:
	inline void SetOwner(std::weak_ptr<gameObject> object) { owner = object.lock(); }
	inline gameObject* GetOwner() { return owner.lock().get(); }
	inline void SetName(const std::wstring& value) { m_componentName = value; }
	inline std::wstring& GetName() { return m_componentName; }

	inline void SetUUID(const std::wstring& value) { m_uuid = value; }
	inline std::wstring& GetUUID() { return m_uuid; }

	const std::wstring& GetOwnerName();
	Transform* GetTransform() const;
	FVector2* GetPivot() const;

protected:
	std::weak_ptr<gameObject> owner;
	std::wstring m_componentName;
	std::wstring m_uuid;
};
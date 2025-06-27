#pragma once
class gameObject;
class Component : public std::enable_shared_from_this<Component>
{
public:
	Component() { SetName(L"Default Component Name"); }
	Component(const std::wstring& name) { SetName(name); }
	virtual ~Component() {};
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;

	virtual void OnStart() {}
	virtual void OnEnd() {}
	virtual void OnCreate() {}
	virtual void OnDestroy() {}

public:
	inline void SetOwner(std::weak_ptr<gameObject> object) { owner = object.lock(); }
	inline gameObject* GetOwner() { return owner.lock().get(); }
	inline std::wstring& GetName() { return m_componentName; }
	inline void SetName(const std::wstring& value) { m_componentName = value; }
	inline std::wstring& GetOwnerName() { return m_ownerObjectName; }
	inline void SetOwnerName(const std::wstring& value) { m_ownerObjectName = value; }
	inline std::wstring& GetUUID() { return m_uuid; }
	inline void SetUUID(const std::wstring& value) { m_uuid = value; }

protected:
	std::weak_ptr<gameObject> owner;
	std::wstring m_componentName;
	std::wstring m_ownerObjectName;
	std::wstring m_uuid;
};
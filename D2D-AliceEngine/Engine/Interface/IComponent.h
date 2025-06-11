#pragma once
class IComponent
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;

public:
	inline std::wstring& GetName() { return m_componentName; }
	inline void SetName(const std::wstring& value) { m_componentName = value; }
	inline std::wstring& GetOwnerName() { return m_ownerObjectName; }
	inline void SetOwnerName(const std::wstring& value) { m_ownerObjectName = value; }
	inline std::wstring& GetUUID() { return m_uuid; }
	inline void SetUUID(const std::wstring& value) { m_uuid = value; }

protected:
	std::wstring m_componentName;
	std::wstring m_ownerObjectName;
	std::wstring m_uuid;
};


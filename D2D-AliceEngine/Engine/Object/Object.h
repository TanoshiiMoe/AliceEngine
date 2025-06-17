#pragma once
class Object
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;

public:
	inline std::wstring& GetName() { return m_objectName; }
	inline void SetName(const std::wstring& value) { m_objectName = value; }

	inline std::wstring& GetUUID() { return m_uuid; }
	inline void SetUUID(const std::wstring& value) { m_uuid = value; }

protected:
	std::wstring m_objectName;
	std::wstring m_uuid;
};


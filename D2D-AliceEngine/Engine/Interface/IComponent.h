#pragma once
class IComponent
{
public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Release();

public:
	inline std::wstring& GetName() { return m_objectName; }
	inline void SetName(wchar_t* value) { m_objectName = value; }

protected:
	std::wstring m_objectName;
	std::wstring m_uuid;
};


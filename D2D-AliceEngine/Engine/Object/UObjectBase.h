#pragma once
#include <string>

class UObjectBase
{
public:
	std::wstring& GetName();
	void SetName(const std::wstring& value);

	std::wstring& GetUUID();
	void SetUUID(const std::wstring& value);

protected:
	std::wstring m_name;
	std::wstring m_uuid;
};


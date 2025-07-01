#pragma once
#include "pch.h"
#include <codecvt>

class StringHelper
{
public:
	static std::wstring MakeUniqueName() {
		static std::atomic<uint64_t> s_counter = 0;
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::wstringstream wss;
		wss << L"_"
			<< std::setfill(L'0')
			<< std::setw(4) << st.wYear
			<< std::setw(2) << st.wMonth
			<< std::setw(2) << st.wDay << L"_"
			<< std::setw(2) << st.wHour
			<< std::setw(2) << st.wMinute
			<< std::setw(2) << st.wSecond
			<< L"_"
			<< std::setw(3) << st.wMilliseconds
			<< L"_" << s_counter++;

		return wss.str();
	}

	static std::string wstring_to_string(const std::wstring& wstr) 
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wstr);
	}

	static std::wstring string_to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(str);
	}
};


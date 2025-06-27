#pragma once
#include "pch.h"

class StringHelper
{
public:
	static std::wstring MakeUniqueName() {
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
			<< std::setw(3) << st.wMilliseconds;

		return wss.str();
	}
};


#pragma once
#include <string>

class StringHelper
{
public:
	StringHelper();
	~StringHelper();

	static std::string WString2String(const std::wstring& str);
};


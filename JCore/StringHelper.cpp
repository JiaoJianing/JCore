#include "stdafx.h"
#include "StringHelper.h"

StringHelper::StringHelper()
{
}


StringHelper::~StringHelper()
{
}

std::string StringHelper::WString2String(const std::wstring& str)
{
	int newLen = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	if (newLen <= 0)
	{
		return std::string("");
		// throw stringT(_T("Can not convert UNICODE string to MULTIBYTE:")) + str;
	}

	char * buf = new char[newLen];

	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buf, newLen, NULL, NULL);

	std::string ret = buf;

	delete[] buf;
	return ret;
}

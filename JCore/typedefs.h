#pragma once
#include <string>

#if defined UNICODE || defined _UNICODE
typedef std::wstring stringT;
typedef wchar_t TCHAR;
typedef std::wifstream ifstreamT;
typedef std::wofstream ofstreamT;
typedef std::wstringstream stringstreamT;

#else

	typedef std::string stringT;
	typedef char TCHAR;
	typedef std::ifstream ifstreamT;
	typedef std::ofstream ofstreamT;
	typedef std::stringstream stringstreamT;
#endif

#ifndef _T

#if defined UNICODE || defined _UNICODE

#define _T(str) L ## str

#else

#define _T(str) str

#endif

#endif
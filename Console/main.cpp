﻿#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <codecvt>
#include <string>

std::wstring utf_to_wstring(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

std::string wstring_to_utf(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

int wmain(int argc, wchar_t* argv[])
{
	std::wstring src = L"鹿島_体";
	std::string dst = wstring_to_utf(src);
	std::wstring backConv = utf_to_wstring(dst);
	
	return 0;
}
#include "helpers.h"

std::wstring ToWStr(const char *str)
{
	std::wstring r;
	for (int i = 0; str[i]; i++)
		r += static_cast<wchar_t>(str[i]);
	return r;
}

std::string ToStr(const wchar_t *str)
{
	std::string r;
	for (int i = 0; str[i]; i++)
		r += static_cast<char>(str[i]);
	return r;
}

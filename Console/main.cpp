#include <iostream>
#include <fstream>
#include <Windows.h>

int wmain(int argc, wchar_t* argv[])
{
	std::wcout << "Arguments:\n";
	for (int i = 0; i < argc; i++)
		std::wcout << i + 1 << ": " << argv[i] << std::endl;
	std::wcout << std::endl;

	if (argc > 1)
		std::wcout << "File to open: " << argv[1] << std::endl;
	else
		std::wcout << "No file to open\n";
	std::wcout << std::endl;

	WCHAR path[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);
	std::wcout << "Executable file path: " << path <<std::endl;
	std::wcout << std::endl;

	std::cin.get();
	return 0;
}
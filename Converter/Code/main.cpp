#include "window.h"
#include <sstream>
#include <iostream>

std::unique_ptr<cvt::Window> g_Window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	if (g_Window)
		g_Window->MessageHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int Main(HINSTANCE hInstance, std::vector<std::wstring>& args)
{
	try
	{
		static LPCWSTR appWindowName = L"Converter";

		SetExeFolderName(hInstance);

		WNDCLASSEX wc{};
		wc.cbSize = sizeof(wc);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = WndProc;
		wc.lpszClassName = appWindowName;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		RegisterClassEx(&wc);

		g_Window = std::make_unique<cvt::Window>(appWindowName, hInstance, args);

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	catch (std::exception e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}

	return 0;
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR szCmdLine, INT nCmdShow)
{
	std::vector<std::wstring> args;
	std::wstringstream ss(szCmdLine);
	std::wstring str;
	while (!ss.eof())
	{
		ss >> str;
		if (!str.empty())
			args.push_back(str);
	}
	return Main(hInstance, args);
}
#include <fstream>
int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> args;
	for (int i = 1; i < argc; i++)
		args.push_back(argv[i]);
	return Main(GetModuleHandle(NULL), args);
}
#include "window.h"

namespace cvt
{
#define CB_POSITION 101
#define CB_TEXTURE 102
#define CB_NORMAL 103
#define CB_NORMALMAP 104
#define CB_BONE 105
#define BTN_EXPORTBIN 111
#define BTN_EXPORTTEXT 112
#define BTN_CLEAR 113


	void Window::DropFileEvent(HDROP hDrop)
	{
		WCHAR filename[MAX_PATH];
		filename[0] = '\0';
		DragQueryFile(hDrop, 0, filename, MAX_PATH);

		try
		{
			SetModel(filename);
		}
		catch (std::exception e)
		{
			MessageBox(m_mainWindow, ToWStr(e.what()).c_str(), L"Error", MB_OK);
		}

		DragFinish(hDrop);
		SetForegroundWindow(m_mainWindow);
	}
	bool Window::isCheckBoxChecked(HWND hwnd)
	{
		return SendMessage(hwnd, BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED;
	}
	void Window::SetCheckBox(HWND hwnd, bool check)
	{
		PostMessage(hwnd, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, LPARAM(0));
	}
	void Window::SetModel(LPCWSTR filename)
	{
		m_modelLoader.Clear();
		m_modelLoader.LoadModel(filename);
		m_scene->SetEntity(m_modelLoader);
		UpdateUserControls();
	}
	void Window::UpdateUserControls()
	{
		InvalidateRect(m_gfxWindow, nullptr, false);
		SetWindowText(m_tbFilename, (m_modelLoader.getFolderName() + m_modelLoader.getFilename() + L".omd").c_str());
		SetCheckBox(m_cbPosition, gfx::ShaderType::HasPositions(m_modelLoader.getShaderType()));
		SetCheckBox(m_cbTexture, gfx::ShaderType::HasTexture(m_modelLoader.getShaderType()));
		SetCheckBox(m_cbNormal, gfx::ShaderType::HasNormals(m_modelLoader.getShaderType()));
		SetCheckBox(m_cbNormalmap, gfx::ShaderType::HasNormalmap(m_modelLoader.getShaderType()));
		SetCheckBox(m_cbBone, gfx::ShaderType::HasBones(m_modelLoader.getShaderType()));
	}
	void Window::ClearModel()
	{
		m_modelLoader.Clear();
		m_scene->ClearEntity();
		InvalidateRect(m_gfxWindow, nullptr, false);
		SetWindowText(m_tbFilename, L"");
		SetCheckBox(m_cbPosition, false);
		SetCheckBox(m_cbTexture, false);
		SetCheckBox(m_cbNormal, false);
		SetCheckBox(m_cbNormalmap, false);
		SetCheckBox(m_cbBone, false);
	}
	void Window::Export(bool binary)
	{
		UINT shaderType = gfx::ShaderType::ToShaderType(
			isCheckBoxChecked(m_cbPosition),
			isCheckBoxChecked(m_cbTexture),
			isCheckBoxChecked(m_cbNormal),
			isCheckBoxChecked(m_cbNormalmap),
			isCheckBoxChecked(m_cbBone));
		WCHAR filename[MAX_PATH];
		GetWindowText(m_tbFilename, filename, MAX_PATH - 1);
		m_modelLoader.ExportOMD(shaderType, filename, binary);
	}
	Window::Window(LPCWSTR appWindowName, HINSTANCE hInstance)
	{
		RECT rect;
		rect.left = 0;
		rect.right = 1280;
		rect.top = 0;
		rect.bottom = 720;
		DWORD wsStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		DWORD wsExStyle = WS_EX_OVERLAPPEDWINDOW;
		AdjustWindowRectEx(&rect, wsStyle, false, wsExStyle);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		m_mainWindow = CreateWindowEx(wsExStyle, appWindowName,
			appWindowName, wsStyle,
			max((GetSystemMetrics(SM_CXSCREEN) - width) / 2, 0),
			max((GetSystemMetrics(SM_CYSCREEN) - height) / 2, 0),
			width, height, nullptr, nullptr, hInstance, nullptr);
		ShowWindow(m_mainWindow, SW_SHOW);
		UpdateWindow(m_mainWindow);
		m_gfxWindow = CreateWindowEx(WS_EX_APPWINDOW, appWindowName,
			appWindowName, WS_VISIBLE | WS_CHILD,
			0, 0, width - 280, height, m_mainWindow, nullptr, nullptr, nullptr);
		DragAcceptFiles(m_gfxWindow, true);

		m_graphics = gfx::Graphics::U(new gfx::Graphics(m_gfxWindow, 1000, 720));
		m_scene = std::make_unique<Scene>(*m_graphics);

		m_tbFilename = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 1030, 10, 240, 20, m_mainWindow, nullptr, nullptr, nullptr);
		m_cbPosition = CreateWindow(L"button", L"Position", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 35, 120, 20, m_mainWindow, (HMENU)CB_POSITION, nullptr, nullptr);
		m_cbTexture = CreateWindow(L"button", L"Texture", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 55, 120, 20, m_mainWindow, (HMENU)CB_TEXTURE, nullptr, nullptr);
		m_cbNormal = CreateWindow(L"button", L"Normal", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 75, 120, 20, m_mainWindow, (HMENU)CB_NORMAL, nullptr, nullptr);
		m_cbNormalmap = CreateWindow(L"button", L"Normalmap", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 95, 120, 20, m_mainWindow, (HMENU)CB_NORMALMAP, nullptr, nullptr);
		m_cbBone = CreateWindow(L"button", L"Bone", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 115, 120, 20, m_mainWindow, (HMENU)CB_BONE, nullptr, nullptr);
		m_btnExportBin = CreateWindow(L"button", L"Export Binary", WS_VISIBLE | WS_CHILD, 1160, 35, 110, 30, m_mainWindow, (HMENU)BTN_EXPORTBIN, nullptr, nullptr);
		m_btnExportText = CreateWindow(L"button", L"Export Text", WS_VISIBLE | WS_CHILD, 1160, 70, 110, 30, m_mainWindow, (HMENU)BTN_EXPORTTEXT, nullptr, nullptr);
		m_btnClear = CreateWindow(L"button", L"Clear", WS_VISIBLE | WS_CHILD, 1160, 105, 110, 30, m_mainWindow, (HMENU)BTN_CLEAR, nullptr, nullptr);

		m_modelLoader.CreateCube(-1.0f, 2.0f, gfx::ShaderType::PN);
		m_scene->SetEntity(m_modelLoader);
		UpdateUserControls();
	}

	void Window::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (hwnd == m_gfxWindow)
		{
			if (m_scene->HandleCamera(msg, wparam, lparam))
				InvalidateRect(hwnd, nullptr, FALSE);
			switch (msg)
			{
			case WM_PAINT:
				ValidateRect(hwnd, nullptr);
				m_scene->Render();
				break;
			case WM_DROPFILES:
				DropFileEvent((HDROP)wparam);
				break;
			}
		}
		if (hwnd == m_mainWindow)
		{
			switch (msg)
			{
			case WM_COMMAND:
				switch (wparam)
				{
				case BTN_CLEAR:
					ClearModel();
					break;
				case BTN_EXPORTBIN:
					Export(true);
					break;
				case BTN_EXPORTTEXT:
					Export(false);
					break;
				}
				break;
			}
		}
	}
}
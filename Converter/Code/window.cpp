#include "window.h"

namespace cvt
{
#define CB_POSITION 101
#define CB_TEXCOORD 102
#define CB_NORMAL 103
#define CB_TGBINORM 104
#define CB_BONE 105
#define CB_TEXTURE 106
#define CB_NORMALMAP 107
#define BTN_EXPORTBIN 111
#define BTN_EXPORTTEXT 112
#define BTN_CLEAR 113
#define CB_SHOWHITBOX 114


	void Window::DropFileEvent(HDROP hDrop, HWND hwnd)
	{
		WCHAR filename[MAX_PATH];
		filename[0] = '\0';
		DragQueryFile(hDrop, 0, filename, MAX_PATH);

		try
		{
			if (hwnd == m_gfxWindow)
				SetModel(filename);
			if (hwnd == m_hitboxDrop)
				SetHitbox(filename);
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
		UINT modelType = gfx::ModelType::ToModelType(
			isCheckBoxChecked(m_cbPosition),
			isCheckBoxChecked(m_cbTexture),
			isCheckBoxChecked(m_cbNormal),
			isCheckBoxChecked(m_cbNormalmap),
			isCheckBoxChecked(m_cbBone),
			true, true);
		m_modelLoader.Clear();
		m_modelLoader.LoadModel(filename, modelType);
		if (m_modelLoader.HasHitbox())
		{
			m_hitboxLoader.Clear();
			m_modelLoader.SwapHitboxes(m_hitboxLoader);
			m_hitboxLoader.MakeVerticesFromHitbox();
			m_modelLoader.SwapHitboxes(m_hitboxLoader);
			m_scene->SetHitbox(m_hitboxLoader);
		}
		m_scene->SetEntity(m_modelLoader);
		UpdateUserControls();
	}
	void Window::SetHitbox(LPCWSTR filename)
	{
		m_hitboxLoader.Clear();
		m_hitboxLoader.LoadModel(filename, gfx::ModelType::P);
		m_hitboxLoader.MakeHitboxFromVertices();
		m_hitboxLoader.SwapHitboxes(m_modelLoader);
		m_scene->SetHitbox(m_hitboxLoader);
		InvalidateRect(m_gfxWindow, nullptr, false);
	}
	void Window::UpdateUserControls()
	{
		InvalidateRect(m_gfxWindow, nullptr, false);
		SetWindowText(m_tbFilename, (m_modelLoader.getFolderName() + m_modelLoader.getFilename() + L".omd").c_str());
		SetCheckBox(m_cbPosition, gfx::ModelType::HasPositions(m_modelLoader.getModelType()));
		SetCheckBox(m_cbTexcoord, gfx::ModelType::HasTexcoords(m_modelLoader.getModelType()));
		SetCheckBox(m_cbNormal, gfx::ModelType::HasNormals(m_modelLoader.getModelType()));
		SetCheckBox(m_cbTangentBinormal, gfx::ModelType::HasTangentsBinormals(m_modelLoader.getModelType()));
		SetCheckBox(m_cbBone, gfx::ModelType::HasBones(m_modelLoader.getModelType()));
		SetCheckBox(m_cbTexture, gfx::ModelType::HasTexture(m_modelLoader.getModelType()));
		SetCheckBox(m_cbNormalmap, gfx::ModelType::HasNormalmap(m_modelLoader.getModelType()));
	}
	void Window::ClearModel()
	{
		m_modelLoader.Clear();
		m_scene->ClearEntity();
		m_hitboxLoader.Clear();
		m_scene->ClearHitbox();
		InvalidateRect(m_gfxWindow, nullptr, false);
		SetWindowText(m_tbFilename, L"");
		SetCheckBox(m_cbPosition, true);
		SetCheckBox(m_cbTexcoord, true);
		SetCheckBox(m_cbNormal, true);
		SetCheckBox(m_cbTangentBinormal, true);
		SetCheckBox(m_cbBone, true);
		SetCheckBox(m_cbTexture, true);
		SetCheckBox(m_cbNormalmap, true);
	}
	void Window::Export(bool binary)
	{
		UINT modelType = gfx::ModelType::ToModelType(
			isCheckBoxChecked(m_cbPosition),
			isCheckBoxChecked(m_cbTexcoord),
			isCheckBoxChecked(m_cbNormal),
			isCheckBoxChecked(m_cbTangentBinormal),
			isCheckBoxChecked(m_cbBone),
			isCheckBoxChecked(m_cbTexture),
			isCheckBoxChecked(m_cbNormalmap));
		WCHAR filename[MAX_PATH];
		GetWindowText(m_tbFilename, filename, MAX_PATH - 1);
		m_modelLoader.ExportOMD(filename, modelType, binary);
	}
	void Window::ShowHitbox()
	{
		m_scene->ShowHitbox(isCheckBoxChecked(m_cbShowHitbox));
		InvalidateRect(m_gfxWindow, nullptr, false);
	}
	Window::Window(LPCWSTR appWindowName, HINSTANCE hInstance, std::vector<std::wstring>& args)
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
		m_cbTexcoord = CreateWindow(L"button", L"Texcoord", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 55, 120, 20, m_mainWindow, (HMENU)CB_TEXCOORD, nullptr, nullptr);
		m_cbNormal = CreateWindow(L"button", L"Normal", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 75, 120, 20, m_mainWindow, (HMENU)CB_NORMAL, nullptr, nullptr);
		m_cbTangentBinormal = CreateWindow(L"button", L"TgBinorm", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 95, 120, 20, m_mainWindow, (HMENU)CB_TGBINORM, nullptr, nullptr);
		m_cbBone = CreateWindow(L"button", L"Bone", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 115, 120, 20, m_mainWindow, (HMENU)CB_BONE, nullptr, nullptr);
		m_cbTexture = CreateWindow(L"button", L"Texture", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 135, 120, 20, m_mainWindow, (HMENU)CB_TEXTURE, nullptr, nullptr);
		m_cbNormalmap = CreateWindow(L"button", L"Normalmap", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1030, 155, 120, 20, m_mainWindow, (HMENU)CB_NORMALMAP, nullptr, nullptr);

		m_btnExportBin = CreateWindow(L"button", L"Export Binary", WS_VISIBLE | WS_CHILD, 1160, 35, 110, 30, m_mainWindow, (HMENU)BTN_EXPORTBIN, nullptr, nullptr);
		m_btnExportText = CreateWindow(L"button", L"Export Text", WS_VISIBLE | WS_CHILD, 1160, 70, 110, 30, m_mainWindow, (HMENU)BTN_EXPORTTEXT, nullptr, nullptr);
		m_btnClear = CreateWindow(L"button", L"Clear", WS_VISIBLE | WS_CHILD, 1160, 105, 110, 30, m_mainWindow, (HMENU)BTN_CLEAR, nullptr, nullptr);
		m_cbShowHitbox = CreateWindow(L"button", L"Show Hitbox", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 1160, 140, 110, 35, m_mainWindow, (HMENU)CB_SHOWHITBOX, nullptr, nullptr);

		m_hitboxDrop = CreateWindow(appWindowName, L"", WS_VISIBLE | WS_CHILD, 1030, 180, 240, 240, m_mainWindow, nullptr, nullptr, nullptr);
		DragAcceptFiles(m_hitboxDrop, true);
		SetCheckBox(m_cbShowHitbox, true);

		if (args.empty())
			m_scene->SetEntityDefaultCube();
		else
		{
			try
			{
				m_modelLoader.LoadModel(args[0].c_str());
				m_scene->SetEntity(m_modelLoader);
			}
			catch (std::exception e)
			{
				m_scene->SetEntityDefaultCube();
				MessageBoxA(NULL, e.what(), "Error", MB_OK);
			}
		}
		//m_modelLoader.MakeHitboxFromVertices();
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
				DropFileEvent((HDROP)wparam, hwnd);
				break;
			}
		}
		if (hwnd == m_hitboxDrop)
		{
			HDC hdc;
			PAINTSTRUCT ps;
			RECT rect;
			switch (msg)
			{
			case WM_PAINT:
				hdc = BeginPaint(hwnd, &ps);
				GetClientRect(hwnd, &rect);
				PatBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, WHITENESS);
				TextOut(hdc, 30, 100, L"Drop hitbox model here...", 25);
				EndPaint(hwnd, &ps);
				break;
			case WM_DROPFILES:
				DropFileEvent((HDROP)wparam, hwnd);
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
				case CB_SHOWHITBOX:
					ShowHitbox();
					break;
				}
				break;
			}
		}
	}
}
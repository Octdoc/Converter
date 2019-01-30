#pragma once

#include "scene.h"
#include <shellapi.h>

namespace cvt
{
	class Window
	{
		HWND m_mainWindow;
		HWND m_gfxWindow;

		HWND m_tbFilename;
		HWND m_cbPosition;
		HWND m_cbTexture;
		HWND m_cbNormal;
		HWND m_cbNormalmap;
		HWND m_cbBone;
		HWND m_btnExportBin;
		HWND m_btnExportText;
		HWND m_btnClear;

		gfx::Graphics::U m_graphics;
		std::unique_ptr<Scene> m_scene;
		gfx::ModelLoader m_modelLoader;

	private:
		void DropFileEvent(HDROP hDrop);
		bool isCheckBoxChecked(HWND hwnd);
		void SetCheckBox(HWND hwnd, bool check);
		void SetModel(LPCWSTR filename);
		void UpdateUserControls();
		void ClearModel();
		void Export(bool binary);
		
	public:
		Window(LPCWSTR appWindowName, HINSTANCE hInstance);

		void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
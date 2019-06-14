#pragma once

#include "scene.h"
#include <shellapi.h>

namespace cvt
{
	class Window
	{
	private:
		HWND m_mainWindow;
		HWND m_gfxWindow;

		HWND m_tbFilename;
		HWND m_cbPosition;
		HWND m_cbTexcoord;
		HWND m_cbNormal;
		HWND m_cbTangentBinormal;
		HWND m_cbBone;
		HWND m_cbTexture;
		HWND m_cbNormalmap;
		HWND m_btnExportBin;
		HWND m_btnExportText;
		HWND m_btnClear;
		HWND m_cbShowHitbox;
		HWND m_hitboxDrop;

		gfx::Graphics::U m_graphics;
		std::unique_ptr<Scene> m_scene;
		gfx::ModelLoader m_modelLoader;
		gfx::ModelLoader m_hitboxLoader;

	private:
		void DropFileEvent(HDROP hDrop, HWND hwnd);
		bool isCheckBoxChecked(HWND hwnd);
		void SetCheckBox(HWND hwnd, bool check);
		void SetModel(LPCWSTR filename);
		void SetHitbox(LPCWSTR filename);
		void UpdateUserControls();
		void ClearModel();
		void Export(bool binary);
		void ShowHitbox();
		
	public:
		Window(LPCWSTR appWindowName, HINSTANCE hInstance, std::vector<std::wstring>& args);

		void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
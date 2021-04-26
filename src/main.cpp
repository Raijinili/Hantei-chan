#include "main.h"
#include "context_gl.h"
#include "main_frame.h"

#include <cstring>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <windows.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <res/resource.h>

ImVec2 clientRect;

static HWND mainWindowHandle;
static ContextGl *context;

void LoadJapaneseFonts(ImGuiIO& io)
{
	char winFolder[512]{};
	ImFontConfig config;
/* 	config.PixelSnapH = 1;
	config.OversampleH = 1;
	config.OversampleV = 1; */
	int appendAt = GetWindowsDirectoryA(winFolder, 512);
	strcpy(winFolder+appendAt, "\\Fonts\\meiryo.ttc");

	if(!io.Fonts->AddFontFromFileTTF(winFolder, 20.0f, &config, io.Fonts->GetGlyphRangesJapanese()))
	{
		auto res = FindResource((HMODULE)GetWindowLongPtr(mainWindowHandle, GWLP_HINSTANCE), MAKEINTRESOURCE(NOTO_SANS_JP_F), RT_RCDATA);
		void *notoFont = LockResource(LoadResource(nullptr, res));
		config.FontDataOwnedByAtlas = false;
		
		io.Fonts->AddFontFromMemoryTTF(notoFont, SizeofResource(nullptr, res), 20.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L, 0L,
		hInstance,
		NULL, NULL, NULL, NULL,
		L"Main window", NULL 
	};

	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, L"HA6GUI: 判定ちゃん６", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);
	mainWindowHandle = hwnd;
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);
	


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = nullptr;
	LoadJapaneseFonts(io);

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init("#version 120");

	MainFrame frame(context);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&frame);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			break;
		}
		frame.Draw();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete context;
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CREATE:
		{
			context = new ContextGl(hWnd);
			if(!gladLoadGL())
			{
				MessageBox(0, L"glad fail", L"gladLoadGL()", 0);
				PostQuitMessage(1);
				return 1;
			}
			return 0;
		}
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			RECT rect;
			GetClientRect(mainWindowHandle, &rect);
			clientRect = ImVec2((float)rect.right, (float)rect.bottom);
			
			MainFrame* mf = (MainFrame*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			mf->UpdateBackProj(glm::ortho<float>(0, clientRect.x/2.f, clientRect.y/2.f, 0));
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

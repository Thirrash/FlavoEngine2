#include "game_core_pch.h"
#include "flavo_game.h"
#include "core/logger/logger.h"
#include "core/platform/os_windows.h"
#include "renderer/render_manager.h"

namespace
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {
				if (MessageBoxA(0, "Are you sure you want to exit?",
					"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
					DestroyWindow(hwnd);
			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}

namespace flavo::game
{
	FlavoGame::FlavoGame(HINSTANCE instance, bool cmd_show)
	{
		bool fullscreen = false;
		int width = 640;
		int height = 480;

		if (fullscreen)
		{
			// Get primary monitor, it is always the one with 0,0 corner in it
			const POINT ptZero = { 0, 0 };
			HMONITOR hmon = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(hmon, &mi);

			width = mi.rcMonitor.right - mi.rcMonitor.left;
			height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		}

		WNDCLASSEX wc;

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "FlavoGame";
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBoxA(NULL, "Error registering window class", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		const HWND hwnd = CreateWindowExA(NULL,
			"FlavoGame",
			"Flavo Game",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL,
			NULL,
			instance,
			NULL);

		if (!hwnd)
		{
			MessageBoxA(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		if (fullscreen)
		{
			SetWindowLongA(hwnd, GWL_STYLE, 0);
		}

		ShowWindow(hwnd, cmd_show);
		UpdateWindow(hwnd);

		renderer::g_RenderManager.Initialize(renderer::ERendererType::DX12);
	}

	int FlavoGame::Loop()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				UpdateGame();
				renderer::g_RenderManager.UpdateRender();
				SyncGameRender();
			}
		}

		return 0;
	}

	void FlavoGame::UpdateGame()
	{
		static int no_frame = 0;
		flavo::logger::debug("Starting game frame: {}", no_frame);
		++no_frame;
	}

	void FlavoGame::SyncGameRender()
	{

	}
}

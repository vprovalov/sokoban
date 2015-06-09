#define VC_EXTRALEAN
#include <Windows.h>
#include "resource.h"

const char *CLASS_NAME = "WinMain";
const char *APP_TITLE = "Sokoban";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
HBITMAP sprites;

char level[20][20] = {
		{ 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 2, 6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 2, 3, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 2, 2, 2, 6, 6, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 2, 6, 6, 3, 6, 3, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 2, 2, 2, 6, 2, 6, 2, 2, 6, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0 },
		{ 2, 6, 6, 6, 2, 6, 2, 2, 6, 2, 2, 2, 2, 2, 6, 6, 1, 1, 2, 0 },
		{ 2, 6, 3, 6, 6, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 2, 0 },
		{ 2, 2, 2, 2, 2, 6, 2, 2, 2, 6, 2, 5, 2, 2, 6, 6, 1, 1, 2, 0 },
		{ 0, 0, 0, 0, 2, 6, 6, 6, 6, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
		{ 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow) 
{
	WNDCLASSEX wcx;
	HWND hwnd;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	if (!RegisterClassEx(&wcx)) return FALSE;

	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hwnd) return FALSE;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return true;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		sprites = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SPRITES));
		break;
	case WM_PAINT:
		{
			//BITMAP bm;
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hWnd, &ps);

			HDC hdcMem = CreateCompatibleDC(hdc);

			SelectObject(hdcMem, sprites);
			//GetObject(sprites, sizeof(bm), &bm);

			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					BitBlt(hdc, i*36, j*36, 36, 36, hdcMem, level[j][i]*36, 0, SRCCOPY);
				}
			}


			DeleteDC(hdcMem);

			EndPaint(hWnd, &ps);
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!CreateMainWindow(hInstance, nCmdShow)) return FALSE;

	while (1) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}

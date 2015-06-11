#define VC_EXTRALEAN
#include <Windows.h>
#include "resource.h"

const char *CLASS_NAME = "WinMain";
const char *APP_TITLE = "Sokoban";
const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 600;
const int LEVEL_WIDTH = 20;
const int LEVEL_HEIGHT = 15;
HBITMAP sprites;
HWND hwnd;

int playerX, playerY;
int crateX, crateY;
int keys;

const char D = 1 << 0;
const char W = 1 << 1;
const char C = 1 << 2;
const char P = 1 << 3;

const int LEFT  = 1 << 0;
const int RIGHT = 1 << 2;
const int UP    = 1 << 3;
const int DOWN  = 1 << 4;

char level[LEVEL_HEIGHT][LEVEL_WIDTH] = {
		{ 0, 0, 0, 0, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, W, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, W, C, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, W, W, W, 0, 0, C, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, W, 0, 0, C, 0, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ W, W, W, 0, W, 0, W, W, 0, W, 0, 0, 0, W, W, W, W, W, W, 0 },
		{ W, 0, 0, 0, W, 0, W, W, 0, W, W, W, W, W, 0, 0, D, D, W, 0 },
		{ W, 0, C, 0, 0, C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, D, D, W, 0 },
		{ W, W, W, W, W, 0, W, W, W, 0, W, P, W, W, 0, 0, D, D, W, 0 },
		{ 0, 0, 0, 0, W, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, W, 0 },
		{ 0, 0, 0, 0, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
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

void Draw(HDC hdc)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, sprites);

	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			int spritePosX = 0;

			if (level[j][i] & W) spritePosX = 2 * 36;
			else if (level[j][i] & C) 
			{
				if (level[j][i] & D)
					spritePosX = 4 * 36;
				else
					spritePosX = 3 * 36;
			}
			else if (level[j][i] & D) spritePosX = 1 * 36;

			BitBlt(hdc, i * 36, j * 36, 36, 36, hdcMem, spritePosX, 0, SRCCOPY);
		}
	}

	BitBlt(hdc, playerX * 36, playerY * 36, 36, 36, hdcMem, 5*36, 0, SRCCOPY);

	DeleteDC(hdcMem);
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CREATE:
		{
			sprites = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SPRITES));			
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			Draw(hdc);
			EndPaint(hWnd, &ps);
		} break;

		case WM_KEYUP:
		{
			switch (wParam) {
				case VK_LEFT:
				{								
					keys &= ~LEFT;
				} break;
				case VK_RIGHT:
				{
					keys &= ~RIGHT;
				} break;
				case VK_UP:
				{
					keys &= ~UP;
				} break;
				case VK_DOWN:
				{
					keys &= ~DOWN;
				} break;
			}
		} break;
		case WM_KEYDOWN:
		{
			switch (wParam) {
				case VK_LEFT:
				{
					keys |= LEFT;
				} break;
				case VK_RIGHT:
				{
					keys |= RIGHT;
				} break;
				case VK_UP:
				{
					keys |= UP;
				} break;
				case VK_DOWN:
				{
					keys |= DOWN;
				} break;
			}
		} break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool UpdateGame() 
{
	int dirX = 0, dirY = 0;

	if (keys & LEFT) 
	{
		dirX = -1;
	}
	else if (keys & RIGHT)
	{
		dirX = 1;
	}
	else if (keys & UP)
	{
		dirY = -1;
	}
	else if (keys & DOWN)
	{
		dirY = 1;
	}
	else
	{
		return false;
	}

	char tmp = level[playerY + dirY][playerX + dirX];

	if ((tmp & W) + (tmp & C) == 0) {
		level[playerY][playerX] &= ~P;
		playerX += dirX;
		playerY += dirY;
		level[playerY][playerX] |= P;
	}
	else if (level[playerY + dirY][playerX + dirX] & C) {
		tmp = level[playerY + dirY + dirY][playerX + dirX + dirX];
		if ((tmp & C) + (tmp & W) == 0) {
			level[playerY][playerX] &= ~P;
			playerX += dirX;
			playerY += dirY;
			level[playerY][playerX] &= ~C;
			level[playerY][playerX] |= P;
			level[playerY + dirY][playerX + dirX] |= C;
		}
	}

	keys = 0;

	return true;
}

void InitGame() 
{
	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			if (level[j][i] == P)
			{
				playerX = i;
				playerY = j;
				break;
			}
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	InitGame();
	if (!CreateMainWindow(hInstance, nCmdShow)) return FALSE;

	while (1) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (UpdateGame())
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
	}
	
	return msg.wParam;
}

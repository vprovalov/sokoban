#define VC_EXTRALEAN
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "resource.h"

const char *CLASS_NAME = "WinMain";
const char *APP_TITLE = "Sokoban";
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int LEVEL_WIDTH = 20;
const int LEVEL_HEIGHT = 16;
const int SPRITE_SIZE = 36;
HBITMAP sprites;
HWND hwnd;

enum GAME_MODE {
	PLAY_LEVEL,
	NEXT_LEVEL
};

int playerX, playerY;
int crateX, crateY;
int keys;
int buttonPressed = 0;
int currentLevel = 0;
int marginX = 0, marginY = 0;
int gameMode = PLAY_LEVEL;

const char D = 1 << 0;
const char W = 1 << 1;
const char C = 1 << 2;
const char P = 1 << 3;

const int LEFT  = 1 << 0;
const int RIGHT = 1 << 2;
const int UP    = 1 << 3;
const int DOWN  = 1 << 4;

char levels[] = {
	0, 0, 0, 0, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, W, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, W, C, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, W, W, 0, 0, C, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, 0, 0, C, 0, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, 0, W, 0, W, W, 0, W, 0, 0, 0, W, W, W, W, W, W, 0,
	W, 0, 0, 0, W, 0, W, W, 0, W, W, W, W, W, 0, 0, D, D, W, 0,
	W, 0, C, 0, 0, C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, D, D, W, 0,
	W, W, W, W, W, 0, W, W, W, 0, W, P, W, W, 0, 0, D, D, W, 0,
	0, 0, 0, 0, W, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, W, 0,
	0, 0, 0, 0, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	W, W, W, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, 0, 0, 0, 0, 0, W, W, W, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, 0, C, 0, 0, C, 0, 0, W, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, C, W, W, W, W, 0, 0, W, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, 0, 0, P, 0, W, W, 0, 0, W, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, 0, W, 0, 0, C, 0, W, W, 0, 0, 0, 0, 0, 0,
	W, W, W, W, W, W, 0, W, W, C, 0, C, 0, W, 0, 0, 0, 0, 0, 0,
	0, 0, W, 0, C, 0, 0, C, 0, C, 0, C, 0, W, 0, 0, 0, 0, 0, 0,
	0, 0, W, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0,
	0, 0, W, W, W, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, P, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, C, W, C, 0, W, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, C, 0, 0, C, W, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, W, C, 0, C, 0, W, 0, 0, 0, 0, 0,
	W, W, W, W, W, W, W, W, W, 0, C, 0, W, 0, W, W, W, 0, 0, 0,
	W, D, D, D, D, 0, 0, W, W, 0, C, 0, 0, C, 0, 0, W, 0, 0, 0,
	W, W, D, D, D, 0, 0, 0, 0, C, 0, 0, C, 0, 0, 0, W, 0, 0, 0,
	W, D, D, D, D, 0, 0, W, W, W, W, W, W, W, W, W, W, 0, 0, 0,
	W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, D, D, D, D, W, 0,
	W, W, W, W, W, W, W, W, W, W, W, W, 0, 0, D, D, D, D, W, 0,
	W, 0, 0, 0, 0, W, 0, 0, C, 0, C, 0, 0, 0, D, D, D, D, W, 0,
	W, 0, C, C, C, W, C, 0, 0, C, 0, W, 0, 0, D, D, D, D, W, 0,
	W, 0, 0, C, 0, 0, 0, 0, 0, C, 0, W, 0, 0, D, D, D, D, W, 0,
	W, 0, C, C, 0, W, C, 0, C, 0, C, W, W, W, W, W, W, W, W, 0,
	W, 0, 0, C, 0, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, 0, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, 0, 0, W, 0, 0, 0, 0, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, 0, 0, 0, C, 0, 0, 0, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, C, C, W, C, C, 0, 0, P, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, 0, 0, W, 0, 0, 0, 0, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, W, W, W, W, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, W, C, W, W, 0, 0, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, C, 0, W, 0, 0, 0,
	W, W, W, W, W, W, W, W, W, 0, W, W, W, 0, 0, 0, W, 0, 0, 0,
	W, D, D, D, D, 0, 0, W, W, 0, C, 0, 0, C, W, W, W, 0, 0, 0,
	W, D, D, D, D, 0, 0, 0, 0, C, 0, C, C, 0, W, W, 0, 0, 0, 0,
	W, D, D, D, D, 0, 0, W, W, C, 0, 0, C, 0, P, W, 0, 0, 0, 0,
	W, W, W, W, W, W, W, W, W, 0, 0, C, 0, 0, W, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, C, 0, C, 0, 0, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, W, W, 0, W, W, 0, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	W, W, W, W, W, W, 0, 0, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, 0, W, W, P, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, W, W, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, 0, 0, 0, C, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, 0, 0, W, 0, W, 0, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, W, W, W, 0, W, 0, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, W, 0, C, 0, W, C, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, W, 0, 0, C, W, 0, C, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, W, 0, C, 0, 0, C, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, W, 0, 0, W, W, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, W, W, W, W, W, W, W, 0, 0, 0, W, W, 0, 0, 0, 0, 0, 0, 0,
	W, W, 0, W, 0, P, W, W, 0, C, C, 0, W, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, 0, 0, C, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, C, 0, 0, W, W, W, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, 0, W, W, W, W, W, 0, W, W, W, 0, 0, 0, 0, 0, 0, 0,
	W, 0, C, 0, 0, W, W, W, 0, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, C, 0, C, 0, C, 0, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, 0, 0, W, W, W, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, C, C, 0, W, 0, W, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, 0, 0, W, W, W, 0, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, 0, 0, W, W, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0,
	0, 0, W, 0, 0, 0, 0, C, 0, 0, 0, C, 0, C, 0, W, 0, 0, 0, 0,
	0, 0, W, 0, C, W, 0, C, 0, W, 0, 0, C, 0, 0, W, 0, 0, 0, 0,
	0, 0, W, 0, 0, C, 0, C, 0, 0, W, 0, 0, 0, 0, W, 0, 0, 0, 0,
	W, W, W, 0, C, W, 0, W, 0, 0, W, W, W, W, 0, W, 0, 0, 0, 0,
	W, P, W, C, 0, C, 0, C, 0, 0, W, W, 0, 0, 0, W, 0, 0, 0, 0,
	W, 0, 0, 0, 0, C, 0, W, C, W, 0, 0, 0, W, 0, W, 0, 0, 0, 0,
	W, 0, 0, 0, C, 0, 0, 0, 0, C, 0, C, 0, C, 0, W, 0, 0, 0, 0,
	W, W, W, W, W, 0, 0, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0,
	0, 0, W, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, D, D, D, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, D, D, D, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, D, D, D, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, D, D, D, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, W, W, W, W, 0, 0, D, D, D, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, D, 0, D, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, 0, 0, W, W, 0, 0, D, D, D, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, W, 0, W, W, 0, 0, D, D, D, W, 0, 0, 0,
	0, 0, 0, 0, 0, W, W, W, 0, W, W, W, W, W, W, W, W, 0, 0, 0,
	0, 0, 0, 0, 0, W, 0, C, C, C, 0, W, W, 0, 0, 0, 0, 0, 0, 0,
	0, W, W, W, W, W, 0, 0, C, 0, C, 0, W, W, W, W, W, 0, 0, 0,
	W, W, 0, 0, 0, W, C, 0, C, 0, 0, 0, W, 0, 0, 0, W, 0, 0, 0,
	W, P, 0, C, 0, 0, C, 0, 0, 0, 0, C, 0, 0, C, 0, W, 0, 0, 0,
	W, W, W, W, W, W, 0, C, C, 0, C, 0, W, W, W, W, W, 0, 0, 0,
	0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, W, W, W, 0, 0, W, W, W, W, W, W, W, W, W, W, W, W, W, 0,
	W, W, P, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, W, 0,
	W, 0, C, C, 0, 0, 0, C, C, 0, 0, C, 0, C, 0, D, D, D, W, 0,
	W, 0, 0, C, C, C, W, 0, 0, 0, 0, C, 0, 0, W, D, D, D, W, 0,
	W, 0, C, 0, 0, 0, W, 0, C, C, 0, C, C, 0, W, D, D, D, W, 0,
	W, W, W, 0, 0, 0, W, 0, 0, C, 0, 0, 0, 0, W, D, D, D, W, 0,
	W, 0, 0, 0, 0, 0, W, 0, C, 0, C, 0, C, 0, W, D, D, D, W, 0,
	W, 0, 0, 0, 0, W, W, W, W, W, W, 0, W, W, W, D, D, D, W, 0,
	W, W, 0, W, 0, 0, W, 0, 0, C, 0, C, 0, 0, W, D, D, D, W, 0,
	W, 0, 0, W, W, 0, W, 0, C, C, 0, C, 0, C, W, W, D, D, W, 0,
	W, 0, D, D, W, 0, W, 0, 0, C, 0, 0, 0, 0, 0, 0, W, D, W, 0,
	W, 0, D, D, W, 0, W, 0, C, C, C, 0, C, C, C, 0, W, D, W, 0,
	W, W, W, W, W, 0, W, 0, 0, 0, 0, 0, 0, 0, W, 0, W, D, W, 0,
	0, 0, 0, 0, W, 0, W, W, W, W, W, W, W, W, W, 0, W, D, W, 0,
	0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, D, W, 0,
	0, 0, 0, 0, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, W, W, W, W, 0, W, 0, 0, W, 0, 0, 0, 0, 0, 0,
	0, 0, 0, W, W, W, 0, P, W, W, W, C, 0, W, 0, 0, 0, 0, 0, 0,
	0, 0, W, W, 0, 0, 0, 0, 0, 0, C, 0, 0, W, 0, 0, 0, 0, 0, 0,
	0, W, W, 0, 0, C, 0, C, C, W, W, 0, W, W, 0, 0, 0, 0, 0, 0,
	0, W, 0, 0, W, C, W, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0,
	0, W, 0, W, 0, C, 0, C, C, 0, W, 0, W, W, W, 0, 0, 0, 0, 0,
	0, W, 0, 0, 0, C, 0, W, 0, 0, W, 0, C, 0, W, W, W, W, W, 0,
	W, W, W, W, 0, 0, 0, 0, W, 0, 0, C, C, 0, W, 0, 0, 0, W, 0,
	W, W, W, W, 0, W, W, 0, C, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, 0,
	W, D, 0, 0, 0, 0, W, W, W, 0, 0, W, W, W, W, W, W, W, W, 0,
	W, D, D, 0, D, D, W, 0, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, D, W, D, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	W, D, D, D, D, D, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0,
	W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0,
	W, 0, W, 0, W, W, W, W, W, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0,
	W, 0, W, 0, 0, C, 0, C, 0, C, 0, C, W, 0, 0, W, 0, 0, 0, 0,
	W, 0, W, 0, 0, 0, C, P, C, 0, 0, 0, W, W, 0, W, W, 0, 0, 0,
	W, 0, W, 0, 0, C, 0, C, 0, C, W, W, W, D, D, D, W, 0, 0, 0,
	W, 0, W, 0, 0, 0, C, 0, C, 0, 0, W, W, D, D, D, W, 0, 0, 0,
	W, 0, W, W, W, C, C, C, 0, C, 0, W, W, D, D, D, W, 0, 0, 0,
	W, 0, 0, 0, 0, 0, W, 0, W, W, 0, W, W, D, D, D, W, 0, 0, 0,
	W, W, W, W, W, 0, 0, 0, W, W, 0, W, W, D, D, D, W, 0, 0, 0,
	0, 0, 0, 0, W, W, W, W, W, 0, 0, 0, 0, 0, W, W, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, W, W, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, W, W, 0, 0, 0, W, W, 0, 0, W, W, W, W, W, W, 0, 0, 0,
	W, W, W, 0, 0, 0, 0, 0, W, 0, 0, W, 0, 0, 0, 0, W, W, W, 0,
	W, 0, 0, C, 0, W, C, 0, W, 0, 0, W, 0, 0, D, D, D, 0, W, 0,
	W, 0, W, 0, C, W, P, C, W, W, 0, W, 0, W, D, W, D, 0, W, 0,
	W, 0, 0, W, 0, W, C, 0, 0, W, 0, 0, 0, 0, D, 0, D, 0, W, 0,
	W, 0, C, 0, 0, 0, 0, C, 0, W, 0, W, 0, W, D, W, D, 0, W, 0,
	W, 0, 0, 0, W, W, 0, 0, W, W, C, 0, C, 0, D, 0, D, 0, W, 0,
	W, 0, C, 0, W, 0, 0, 0, W, 0, 0, W, C, W, D, W, D, 0, W, 0,
	W, W, 0, C, 0, 0, C, 0, 0, 0, C, 0, 0, C, D, D, D, 0, W, 0,
	0, W, C, 0, W, W, W, W, W, W, 0, 0, 0, 0, W, W, 0, 0, W, 0,
	0, W, 0, 0, W, 0, 0, 0, 0, W, W, W, W, W, W, W, W, W, W, 0,
	0, W, W, W, W, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, 0,
	0, W, W, W, W, W, W, W, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, 0,
	0, W, 0, 0, 0, 0, 0, W, 0, C, P, C, 0, W, 0, 0, 0, 0, 0, 0,
	0, W, C, C, 0, W, 0, 0, 0, W, W, W, W, W, W, W, W, W, 0, 0,
	0, W, 0, W, W, W, D, D, D, D, D, D, W, W, 0, 0, 0, W, 0, 0,
	0, W, 0, 0, 0, C, D, D, D, D, D, D, W, W, 0, W, 0, W, 0, 0,
	0, W, 0, W, W, W, D, D, D, D, D, D, 0, 0, 0, 0, 0, W, 0, 0,
	W, W, 0, 0, 0, W, W, W, W, 0, W, W, W, 0, W, C, W, W, 0, 0,
	W, 0, 0, W, C, 0, 0, 0, W, 0, 0, C, 0, 0, W, 0, W, 0, 0, 0,
	W, 0, 0, C, 0, C, C, C, 0, 0, W, 0, C, W, W, 0, W, 0, 0, 0,
	W, 0, 0, 0, C, 0, C, 0, W, W, W, C, C, 0, W, 0, W, 0, 0, 0,
	W, W, W, W, W, 0, 0, 0, 0, 0, C, 0, 0, 0, W, 0, W, 0, 0, 0,
	0, 0, 0, 0, W, W, W, 0, W, W, W, 0, 0, 0, W, 0, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, 0, 0, 0, 0, 0, W, 0, 0, 0, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, W, W, W, W, W, W, W, W, 0, 0, W, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, W, W, W, W, 0, 0, 0,
};

int numLevels = sizeof(levels) / (LEVEL_HEIGHT * LEVEL_WIDTH);
char level[LEVEL_HEIGHT * LEVEL_WIDTH];

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
void LoadLevel();
void InitGame();

BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow) 
{
	WNDCLASSEX wcx;
	
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	if (!RegisterClassEx(&wcx)) return FALSE;

	HWND hWnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

void DrawLevel(HDC hdc)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, sprites);

	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			char entity = level[j*LEVEL_WIDTH + i];
			int spritePosX = 6*SPRITE_SIZE;

			if (entity & W) spritePosX = 2 * SPRITE_SIZE;
			else if (entity & C)
			{
				if (entity & D)
					spritePosX = 4 * SPRITE_SIZE;
				else
					spritePosX = 3 * SPRITE_SIZE;
			}
			else if (entity & D) spritePosX = 1 * SPRITE_SIZE;

			BitBlt(hdc, 
				marginX + i * SPRITE_SIZE, 
				marginY + j * SPRITE_SIZE, 
				SPRITE_SIZE, 
				SPRITE_SIZE, 
				hdcMem, 
				spritePosX, 
				0, 
				SRCCOPY);
		}
	}

	BitBlt(
		hdc, 
		marginX + playerX * SPRITE_SIZE, 
		marginY + playerY * SPRITE_SIZE, 
		SPRITE_SIZE, 
		SPRITE_SIZE, 
		hdcMem, 
		5 * SPRITE_SIZE, 
		0, 
		SRCCOPY);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	BitBlt(
		hdc,
		clientRect.right - clientRect.left - SPRITE_SIZE * 3,
		clientRect.bottom - clientRect.top - SPRITE_SIZE,
		SPRITE_SIZE * 3,
		SPRITE_SIZE,
		hdcMem,
		7 * SPRITE_SIZE,
		0,
		SRCCOPY);

	DeleteDC(hdcMem);
}

void Draw(HDC hdc)
{
	switch (gameMode)
	{
	case PLAY_LEVEL:
		DrawLevel(hdc);
		break;
	case NEXT_LEVEL:
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		break;
	}
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
			hwnd = hWnd;
			InitGame();
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			Draw(hdc);
			EndPaint(hWnd, &ps);
		} break;

		case WM_LBUTTONUP:
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			int menuX = clientRect.right - clientRect.left - SPRITE_SIZE * 3;
			int menuY = clientRect.bottom - clientRect.top - SPRITE_SIZE;

			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			if (xPos > menuX && xPos < clientRect.right - clientRect.left &&
				yPos > menuY && yPos < clientRect.bottom - clientRect.top) 
			{
				buttonPressed = 1 + ((xPos - menuX) / SPRITE_SIZE);
			}
		}

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
	bool invalidateScreen = false;

	if (gameMode == PLAY_LEVEL)
	{	
		int dirX = 0, dirY = 0;

		if (buttonPressed > 0) 
		{					
			switch (buttonPressed)
			{
			case 1:
				if (currentLevel > 0) 
				{
					invalidateScreen = true;
					currentLevel--;
				}
				break;
			case 2:
				invalidateScreen = true;
				break;
			case 3:
				if (currentLevel + 1 < numLevels)
				{
					invalidateScreen = true;
					currentLevel++;
				}				
				break;
			}
			
			if (invalidateScreen) gameMode = NEXT_LEVEL;
			
			buttonPressed = 0;
		} else if (keys > 0) 
		{
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

			char tmp = level[(playerY + dirY)*LEVEL_WIDTH + playerX + dirX];

			if ((tmp & W) + (tmp & C) == 0)
			{
				level[playerY*LEVEL_WIDTH + playerX] &= ~P;
				playerX += dirX;
				playerY += dirY;
				level[playerY*LEVEL_WIDTH + playerX] |= P;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

				invalidateScreen = true;
			}
			else if (level[(playerY + dirY) * LEVEL_WIDTH + playerX + dirX] & C)
			{
				tmp = level[(playerY + dirY + dirY) * LEVEL_WIDTH + playerX + dirX + dirX];
				if ((tmp & C) + (tmp & W) == 0)
				{
					level[playerY*LEVEL_WIDTH + playerX] &= ~P;
					playerX += dirX;
					playerY += dirY;
					level[playerY*LEVEL_WIDTH + playerX] &= ~C;
					level[playerY*LEVEL_WIDTH + playerX] |= P;
					level[(playerY + dirY)*LEVEL_WIDTH + playerX + dirX] |= C;

					if ((level[playerY*LEVEL_WIDTH + playerX] & D) == 0 &&
						level[(playerY + dirY)*LEVEL_WIDTH + playerX + dirX] & D)
					{
						PlaySound(MAKEINTRESOURCE(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
					}
					else
					{
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
					}

					invalidateScreen = true;
				}
			}

			keys = 0;
		}

		int i = 0;
		for (; i < LEVEL_HEIGHT*LEVEL_WIDTH; i++)
		{
			if (level[i] & C && (level[i] & D) == 0) break;
		}

		if (i == LEVEL_HEIGHT*LEVEL_WIDTH && currentLevel < numLevels - 1)
		{
			currentLevel++;
			gameMode = NEXT_LEVEL;
			PlaySound(MAKEINTRESOURCE(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

			invalidateScreen = true;
		}
	} else if (gameMode == NEXT_LEVEL)
	{
		LoadLevel();
		gameMode = PLAY_LEVEL;
		invalidateScreen = true;
	}

	return invalidateScreen;
}

void LoadLevel()
{
	memcpy(level, levels + LEVEL_HEIGHT*LEVEL_WIDTH*currentLevel, LEVEL_HEIGHT*LEVEL_WIDTH*sizeof(char));

	int width = 0, height = 0;
	for (int i = 0; i < LEVEL_HEIGHT*LEVEL_WIDTH; i++)
	{
		if (level[i] > 0)
		{
			width = max(width, i % LEVEL_WIDTH);
			height = max(height, i / LEVEL_WIDTH);
		}

		if (level[i] == P)
		{
			playerX = i % LEVEL_WIDTH;
			playerY = i / LEVEL_WIDTH;
		}
	}

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	marginX = ((clientRect.right - clientRect.left) - (width + 1) * SPRITE_SIZE) / 2;
	marginY = ((clientRect.bottom - clientRect.top) - (height + 1) * SPRITE_SIZE) / 2;

	wchar_t title[20];
	swprintf_s(title, 20, L"Sokoban - Level%d", currentLevel + 1);
	SetWindowTextW(hwnd, title);
}

void InitGame() 
{
	LoadLevel();
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

		if (UpdateGame())
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
	}
	
	return msg.wParam;
}

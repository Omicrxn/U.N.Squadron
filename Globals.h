#pragma once
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>

#define LOG(format, ...) Log(__FILE__,__LINE__,format,__VA_ARGS__);
void Log(const char file[], int line, const char* format, ...);
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

typedef unsigned char uchar;
typedef unsigned int uint;


// Settings
#define WINDOW_ICON_PATH "Assets/Sprites/windowIcon.bmp"
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 448
#define WIN_FULLSCREEN 0
#define WIN_FULLSCREEN_DESKTOP 0
#define SCREEN_SIZE 2
#define WIN_BORDERLESS 0
#define WIN_RESIZABLE 1
#define REN_VSYNC 1
#define WIN_TITLE "U.N. Squadron"

#endif // __GLOBALS_H__
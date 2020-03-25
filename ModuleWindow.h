#pragma once
#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{

public:
	ModuleWindow();

	virtual ~ModuleWindow();

	bool Init();

	bool CleanUp();

	SDL_Window* sdl_window = nullptr;
	SDL_Surface* screen_surface = nullptr;

	SDL_Surface* windowIcon;


};

#endif // __ModuleWindow_H__
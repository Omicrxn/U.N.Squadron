#pragma once
#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{

public:
	ModuleWindow(bool startEnabled);

	virtual ~ModuleWindow();

	bool Init();

	bool CleanUp();

	SDL_Window* sdlWindow = nullptr;
	SDL_Surface* screenSurface = nullptr;

	SDL_Surface* windowIcon;


};

#endif // __ModuleWindow_H__
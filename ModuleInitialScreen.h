#pragma once
#ifndef __MODULEINITIALSCREEN_H__
#define __MODULEINITIALSCREEN_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_timer.h"
#include "SDL_rect.h"
struct SDL_Texture;
class ModuleInitialScreen : public Module
{
public:
	ModuleInitialScreen(bool startEnabled);
	~ModuleInitialScreen();
	
	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* logoTex = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Rect logo;
	SDL_Rect screen;
	
	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 actualTime = 0;
};
#endif


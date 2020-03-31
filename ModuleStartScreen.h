#pragma once
#ifndef __MODULESTARTSCREEN_H__
#define __MODULESTARTSCREEN_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
class ModuleStartScreen : public Module
{
public:
	ModuleStartScreen();
	~ModuleStartScreen();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
private:
	SDL_Texture* tex = nullptr;
	SDL_Rect title;
	SDL_Rect text;
	SDL_Rect selector;
	Animation unicorn_anim;
	Animation* current_anim;
	iPoint selectorPos;
};
#endif

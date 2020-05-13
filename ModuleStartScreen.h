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
	ModuleStartScreen(bool startEnabled);
	~ModuleStartScreen();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	// Sound effects indices
	uint chooseFx = 0;
	uint startFx = 0;

private:
	SDL_Texture* tex = nullptr;
	SDL_Rect title;
	SDL_Rect text;
	Animation selector;
	Animation unicorn_anim;
	Animation* current_anim = nullptr;
	iPoint selectorPos;
	SDL_Rect rect;
	bool blink = false;
};
#endif

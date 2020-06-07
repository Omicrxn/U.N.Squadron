#pragma once
#ifndef __MODULESELECTOR_H__
#define __MODULESELECTOR_H__

#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "p2Point.h"

enum selection {
	NONE, LEVEL1, LEVEL2
};

struct SDL_Texture;

class ModuleSelector : public Module
{
public:
	ModuleSelector(bool startEnabled);
	~ModuleSelector();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	// Id Fonts
	int greyFont = -1;

	// Selection index
	uint selected = NONE;

private:
	SDL_Texture* tex1 = nullptr;
	SDL_Texture* tex2 = nullptr;
	SDL_Texture* tex3 = nullptr;

	SDL_Rect background;
};
#endif
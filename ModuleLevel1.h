#pragma once

#ifndef __MODULELEVEL1_H__
#define __MODULELEVEL1_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_rect.h"
#include "p2Point.h"

struct SDL_Texture;
class ModuleLevel1 : public Module
{
public:
	ModuleLevel1(bool startEnabled);
	~ModuleLevel1();

	bool Start();

	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void InfiniteScrolling(fPoint* top, fPoint* top2, SDL_Rect* topLayer, float speed);

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect sky;
	SDL_Rect mountains;
	

	fPoint mountainsPos;
	fPoint mountainsTwoPos;
	float mountainsSpeed;

};

#endif
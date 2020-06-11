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

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect sky;
	SDL_Rect mountains;
	SDL_Rect floor;

	fPoint skyPos, mountainsPos, floorPos;
	fPoint skyPos2, mountainsPos2, floorPos2;
	float mountainsSpeed, skySpeed, floorSpeed;

};

#endif
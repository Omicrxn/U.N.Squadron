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
	SDL_Rect skyHalf1;
	SDL_Rect skyHalf2;
	SDL_Rect mountainsHalf1;
	SDL_Rect mountainsHalf2;
	SDL_Rect floorHalf1;
	SDL_Rect floorHalf2;

	unsigned int numSpawnedEnemies = 0;

private:
	enum position {
		LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN
	};

	enum direction
	{
		LEFTDIR, RIGHTDIR
	};

	uint currentPosition;
	uint currentDirection;
	uint cameraSpeed = 7;
	uint proportion = 3;
};

#endif
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
	SDL_Texture* backgroundTexture2 = nullptr;
	SDL_Rect sky;
	SDL_Rect mountains;
	SDL_Rect floor;

	unsigned int numSpawnedEnemies = 0;

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

	bool spinningRight;
	uint spinningRightCounter;

	bool spinningLeft;
	uint spinningLeftCounter;
};

#endif
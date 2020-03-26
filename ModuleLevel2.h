#pragma once

#ifndef __MODULELEVEL2_H__
#define __MODULELEVEL2_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_rect.h"

struct SDL_Texture;
class ModuleLevel2 : public Module
{
public:
	ModuleLevel2();
	~ModuleLevel2();

	bool Start();
	update_status Update();
	bool CleanUp();

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect background;
	SDL_Rect upperClouds;
	SDL_Rect lowerClouds;

	float background_x;
	float background_y;
	float background2_x;
	float background2_y;
	float background_speed;

	float upperClouds_x;
	float upperClouds_y;
	float upperClouds2_x;
	float upperClouds2_y;
	float upperClouds_speed;

	float lowerClouds_y;
	float lowerClouds2_y;

};

#endif
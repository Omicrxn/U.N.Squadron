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

	void InfiniteScrolling(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2, SDL_Rect* topLayer, SDL_Rect* botLayer, float speed);

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect firsttopLayer;
	SDL_Rect secondtopLayer;
	SDL_Rect thirdtopLayer;
	SDL_Rect firstBottomLayer;
	SDL_Rect secondBottomLayer;

	fPoint firsttopPos, secondTopPos, firstBotPos, secondBotPos;
	fPoint firsttopPos2, secondTopPos2, firstBotPos2, secondBotPos2;
	float topSpeed, firstSpeed, secondSpeed, thirdSpeed;

};

#endif
#pragma once

#ifndef __MODULELEVEL2_H__
#define __MODULELEVEL2_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_rect.h"
#include "p2Point.h"

struct SDL_Texture;
class ModuleLevel2 : public Module
{
public:
	ModuleLevel2();
	~ModuleLevel2();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Parallax(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2, SDL_Rect* topLayer, SDL_Rect* botLayer, float speed);

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect centerLayer;
	SDL_Rect firstTopLayer;
	SDL_Rect secondTopLayer;
	SDL_Rect thirdTopLayer;
	SDL_Rect firstBottomLayer;
	SDL_Rect secondBottomLayer;
	SDL_Rect thirdBottomLayer;

	fPoint centerPos,firstTopPos,secondTopPos,thirdTopPos,firstBotPos, secondBotPos, thirdBotPos;
	fPoint centerPos2,firstTopPos2,secondTopPos2,thirdTopPos2,firstBotPos2, secondBotPos2, thirdBotPos2;
	float centerSpeed, firstSpeed, secondSpeed, thirdSpeed;
	

};

#endif
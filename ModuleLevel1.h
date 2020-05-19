#pragma once

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

	//update_status Update();
	//update_status PostUpdate();
	//bool CleanUp();

	void InfiniteScrolling(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2, SDL_Rect* topLayer, SDL_Rect* botLayer, float speed);

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect topLayer;
	SDL_Rect firstBottomLayer;
	SDL_Rect secondBottomLayer;
	SDL_Rect bossFirstBottomLayer;
	SDL_Rect bossSecondBottomLayer;
	SDL_Rect buildingsBottomLayer;

	fPoint centerPos, toplLayerPos, firstBottomLayerPos, secondBottomLayerPos, thirdBottomLayerPos;
	fPoint centerPos2, topLayerPos2, firstBottomLayerPos2, secondBottomLayerPos2, thirdBottomLayerPos2;
	float centerSpeed, firstSpeed, secondSpeed, thirdSpeed;

	unsigned int numSpawnedEnemies = 0;
};
#pragma once

#ifndef __MODULE_HUD_H__
#define __MODULE_HUD_H__

#include "Module.h"
#include "Animation.h"
struct SDL_Texture;
class ModuleHUD : public Module
{
public:
	ModuleHUD(bool startEnabled);

	~ModuleHUD();

	bool Start();

    update_status Update();
	update_status PostUpdate();

	bool CleanUp();

	char scoreText[150] = { "\0" };
	char moneyText[10] = { "\0" };
	char lifesText[10] = { "\0" };

	char lookupTable[91] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };

	// Id Fonts
	int yellowFont = -1;
	int greenFont = -1;

private:
	SDL_Texture* tex;
	Animation playerFace;
	Animation dangerAnim;
	SDL_Rect fuelBackground;
	SDL_Rect fuelQuantity;
	SDL_Rect helmet;
	SDL_Rect powSquare;
	SDL_Rect bomb;
	SDL_Rect shell;
};

#endif // __ModuleHUD_H__
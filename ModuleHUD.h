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

	char lookupTable[91] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };

private:
	SDL_Texture* tex;
	SDL_Texture* tex2;
	Animation playerFace;
	Animation dangerAnim;
	Animation playerIn;
	Animation playerOut;
	Animation playerHasBeenHit;
	SDL_Rect fuelBackground;
	SDL_Rect fuelQuantity;
	SDL_Rect helmet;
	SDL_Rect powSquare;
	SDL_Rect bomb;
	SDL_Rect shell;
	SDL_Rect falcon;
	SDL_Rect ceiling;

	char falconAmmoText[5] = { '\0' };
	char shellAmmoText[5] = {'\0'};
	char bombAmmoText[5] = { '\0' };
	char ceilingAmmoText[5] = { '\0' };

	char scoreText[150] = { "\0" };
	char moneyText[10] = { "\0" };
	char lifesText[10] = { "\0" };

	char powText[4] = { "\0" };
	char totalText[4] = { "\0" };
	char levelText[2] = { "\0" };

	// Id Fonts
	int yellowFont = -1;
	int greenFont = -1;
};

#endif // __ModuleHUD_H__
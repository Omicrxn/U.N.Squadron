#pragma once
#ifndef __MODULESTORE_H__
#define __MODULESTORE_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
class ModuleStore : public Module
{
public:
	ModuleStore(bool startEnabled);
	~ModuleStore();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	char moneyText[10] = { "\0" };

	// Id Fonts
	int greyFont = -1;
	int greenFont = -1;

	// Sound effects indices
	uint chooseFx = 0;

	// Bit field to store all the weapons selected to start the game
	// The last 11 bits represent each weapon selectable for the player (from left to right)
	// For instance, 00000000 00000000 00000000 00000001 means that WEAPON_11 has been selected
	// With that we'll be able to know which weapons have been selected by the player without wasting memory unnecessarily :)
	uint weaponSelection = 0; // 00000000 00000000 00000000 00000000

private:
	SDL_Texture* tex = nullptr;
	SDL_Texture* tex2 = nullptr;
	SDL_Texture* tex3 = nullptr;
	SDL_Texture* tex4 = nullptr;
	SDL_Rect title;
	SDL_Rect text;
	SDL_Rect selector;
	SDL_Rect background;
	SDL_Rect alreadySelected;
	Animation talkStore;
	Animation unicorn_anim;
	Animation* current_anim = nullptr;
	iPoint selectorPos;

	// Countdown to handle selector spacing
	int MaxCountdownR = 15;
	int CountdownR = 0;

	int MaxCountdownL = 15;
	int CountdownL = 0;

	// A few variables to manage the correct functioning of the store...
	// Not necessary for the remaining modules (because of that they are not defined in the .h)
	enum weapons {
		WEAPON_1, WEAPON_2, FALCON, WEAPON_4, SHELL, WEAPON_6,
		BOMB, WEAPON_8, CEILING, WEAPON_10, WEAPON_11, EXIT
	};

	uint weapons[2][6] = { {WEAPON_1, WEAPON_2, FALCON, WEAPON_4, SHELL, WEAPON_6},
						   {BOMB, WEAPON_8, CEILING, WEAPON_10, WEAPON_11, EXIT} };

	uint rows = 0;
	uint columns = 0;
	uint weapon = weapons[rows][columns];
	///////////////////////////////////////////////////////////////////////////////////////////

	enum storeState {
		FALCONSELECT, SHELLSELECT, BOMBSELECT, CEILINGSELECT, CANTHANDLE, BOUGHT, ALREADYBOUGHT, NOMONEY, EXITSELECT, BYE, IDLE
	};

	storeState currentState = CANTHANDLE;

	uint storeStateCounter = 0;
};
#endif // __MODULESTORE_H__
#pragma once
#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL_rect.h"
#include "Animation.h"

struct SDL_Texture;

class ModulePlayer : public Module {
public:
	ModulePlayer(bool startEnabled);
	~ModulePlayer();
	bool Start();
	bool Init();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	// The player's collider
	Collider* collider = nullptr;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// Sound effects indices
	uint shootFx = 0;
	uint explosionFx = 0;

	// Score
	uint score = 0;

	// Money
	uint money = 3000;

	iPoint GetPlayerPosition() {
		return position;
	}
	int GetCurrentFuel() {
		return currentFuel;
	}
	int GetMaxFuel() {
		return maxFuel;
	}
	int GetLifes() {
		return playerLifes;
	}

	// Bit field to store all the weapons selected to start the game
	// The last 11 bits represent each weapon selectable for the player (from left to right)
	// For instance, 00000000 00000000 00000000 00000001 means that WEAPON_11 has been selected
	// With that we'll be able to know which weapons have been selected by the player without wasting memory unnecessarily :)
	uint weaponSelection = 0; // 00000000 00000000 00000000 00000000

private:
	iPoint position;
	SDL_Texture* texture = nullptr;
	Animation* current_anim = nullptr;
	Animation playerAnim;
	SDL_Rect rectAnim;
	int maxFuel = 8, currentFuel = 8, playerLifes = 3;
	bool godMode = false;
};

#endif // __MODULEPLAYER_H__
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
	uint hitFx = 0;
	uint dieFx = 0;

	// Score
	uint score = 0;

	// Money
	uint money = 3000;

	// Total POW
	uint total = 0;

	// POW Level
	uint level = 1;

	bool maxAmmo = false;

	bool maxPow = false;

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
	uint GetCurrentPOW();

	// The whole list of weapons to know which one is the player selecting at each moment
	enum weapons {
		WEAPON_1, WEAPON_2, FALCON, WEAPON_4, SHELL, WEAPON_6,
		BOMB, WEAPON_8, CEILING, WEAPON_10, WEAPON_11, NONE
	};

	// To know which is the current weapon (the last one that has been selected)
	uint currentWeapon;

	uint falconAmmo;
	uint shellAmmo;
	uint bombAmmo;
	uint ceilingAmmo;

	bool hasBeenHit;
	uint hasBeenHitCounter;
private:
	iPoint position;
	SDL_Texture* texture = nullptr;
	Animation* current_anim = nullptr;
	Animation playerAnim;
	SDL_Rect rectAnim;
	int maxFuel = 8, currentFuel = 8, playerLifes = 3;
	bool godMode = false;
	uint weaponCount;
	bool hasBought;

	// Countdown to handle shot spacing
	int shotMaxCountdown = 10;
	int shotCountdown = 0;

	// Countdown to handle weapon shot spacing
	int weaponMaxCountdown = 20;
	int weaponCountdown = 0;

	// Countdown to handle weapon change spacing
	int changeMaxCountdown = 25;
	int changeCountdown = 0;

	// Ceiling countdown
	uint ceilingCountdown;

	// POW
	uint pow1 = 4, pow2 = 11, pow3 = 13, pow4 = 16;

	friend class ModuleLevel1;
};

#endif // __MODULEPLAYER_H__
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

private:
	iPoint position;
	SDL_Texture* texture = nullptr;
	Animation* current_anim = nullptr;
	Animation playerAnim;
	SDL_Rect rectAnim;
	bool godMode;
};

#endif // __MODULEPLAYER_H__
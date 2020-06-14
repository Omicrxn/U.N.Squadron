#pragma once

#include "Enemy.h"
#include "Path.h"
enum GFState {
	gSPAWNING, gIDLE
};
class GreenFighterPlane : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	GreenFighterPlane(int x, int y, bool spawnRight);

	// The enemy is going to follow different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	Path spawnPathTop;
	Path spawnPathBot;
	Path idlePathTop;
	Path idlePathBot;
	Animation idle;
	Animation shooting;
	SDL_Rect rect;
	unsigned int shootingFrequency;
	GFState state = gSPAWNING;
	bool spawningTop = false;
	bool goingUp = false;
	iPoint idlePosition = { 0, 0 };
	
};
#pragma once
#include "Enemy.h"
#include "Path.h"

class Truck : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Truck(int x, int y, bool spawnRight);

	// The enemy is going to follow different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies
	Animation truckAnimation;

	Path path;
};
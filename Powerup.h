#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "Weapon.h"
#include "Path.h"

class Powerup : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Powerup(int x, int y);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;

	Animation animPower;
};

#endif // __POWERUP_H__#pragma once

#ifndef __POWERUPBLUE_H__
#define __POWERUPBLUE_H__

#include "Weapon.h"
#include "Path.h"

class PowerupBlue : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	PowerupBlue(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path pathBlue;

	Animation animPowerBlue;
};

#endif // __POWERUPBLUE_H__#pragma once
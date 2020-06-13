#ifndef __POWERUPORANGE_H__
#define __POWERUPORANGE_H__

#include "Weapon.h"
#include "Path.h"

class PowerupOrange : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	PowerupOrange(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path pathOrange;

	Animation animPowerOrange;
};

#endif // __POWERUPORANGE_H__#pragma once#pragma once

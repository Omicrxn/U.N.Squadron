#pragma once
#ifndef __GF_HOOK_H__
#define __GF_HOOK_H__


#include "Weapon.h"
#include "Path.h"
class GF_Hook : public Weapon
{

public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	GF_Hook(int x, int y);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;

	// This weapon has one sprite and one frame
	// We are keeping it an animation for consistency with other weapons
	Animation drop;
};
#endif // !__GF_HOOK_H__

#ifndef __CEILING_H__
#define __CEILING_H__

#include "Weapon.h"
#include "Path.h"

class Ceiling : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Ceiling(int x, int y);

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

#endif // __CEILING_H__
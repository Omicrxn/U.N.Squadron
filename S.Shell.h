#ifndef __SHELL_H__
#define __SHELL_H__

#include "Weapon.h"
#include "Path.h"

class Shell : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Shell(int x, int y);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;

	// This weapon has one sprite and one frame
	// We are keeping it an animation for consistency with other weapons
	Animation dropRight1;
	Animation dropRight2;
	Animation dropLeft1;
	Animation dropLeft2;
};

#endif // __S.SHELL_H__
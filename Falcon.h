#ifndef __FALCON_H__
#define __FALCON_H__

#include "Weapon.h"
#include "Path.h"

class Falcon : public Weapon
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Falcon(int x, int y);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;

	// This weapon has one sprite and one frame
	// We are keeping it an animation for consistency with other weapons
	Animation dropRight1;
	Animation dropRight2;
	Animation dropRight3;
	Animation dropLeft1;
	Animation dropLeft2;
	Animation dropLeft3;
};

#endif // __FALCON_H__
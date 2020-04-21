#ifndef __MEDIUMCAMOUFLAGEJET_H__
#define __MEDIUMCAMOUFLAGEJET_H__

#include "Enemy.h"
#include "Path.h"

class MediumCamouflageJet : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	MediumCamouflageJet(int x, int y,bool spawnRight);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;

	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies
	Animation flyLeft;
	Animation turnRight;
	Animation flyRight;

	unsigned int shootingFrequency;
};

#endif // __MEDIUMCAMOUFLAGEJET_H__
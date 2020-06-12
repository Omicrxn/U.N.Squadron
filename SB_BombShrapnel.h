#pragma once
#ifndef __SB_BOMBSHRAPNEL_H__
#define __SB_BOMBSHRAPNEL_H__

#include "Weapon.h"
#include "Path.h"
class SB_BombShrapnel : public Weapon
{

public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	SB_BombShrapnel(int x, int y,unsigned int number);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	//The paths start from the horizontal left and go clockwise
	//              2
	//            1   3
	//           0     4
	//            7   5
	//              6
	//
	Path path0;
	Path path1;
	Path path2;
	Path path3;
	Path path4;
	Path path5;
	Path path6;
	Path path7;

	// This weapon has one sprite and one frame
	// We are keeping it an animation for consistency with other weapons
	Animation shrapnel;
	unsigned int shrapnelNum = 0;
};
#endif // !__SB_BOMBSHRAPNEL_H__


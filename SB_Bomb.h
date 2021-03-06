#pragma once
#ifndef __SB_BOMB_H__
#define __SB_BOMB_H__

#include "Weapon.h"
#include "Path.h"
class SB_Bomb : public Weapon
{

public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	SB_Bomb(int x, int y,unsigned int number);

	// The weapon is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;
	void Explosion(Path* path);
private:
	unsigned int bombNum;
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path0;
	Path path1;

	// This weapon has one sprite and one frame
	// We are keeping it an animation for consistency with other weapons
	Animation bomb;
};
#endif // !__SB_BOMB_H__

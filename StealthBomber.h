#pragma once
#ifndef _STEALTHBOMBER_H__
#define _STEALTHBOMBER_H__
#include "Enemy.h"
#include "Path.h"
class StealthBomber : public Enemy
{
	StealthBomber(int x, int y, bool spawnRight);
	void Update() override;
	Path path;
	Animation damaged;
	Animation shooting;
	SDL_Rect rect;
	unsigned int shootingFrequency;

};
#endif // !_STEALTHBOMBER_H_




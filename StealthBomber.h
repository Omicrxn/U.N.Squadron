#pragma once
#ifndef _STEALTHBOMBER_H__
#define _STEALTHBOMBER_H__
#include "Enemy.h"
#include "Path.h"
class StealthBomber : public Enemy
{
public:
	StealthBomber(int x, int y, bool spawnRight);
	void Update() override;
private:
	Path spawnPath;
	Path normalPath;
	Animation idle;
	Animation shooting;
	SDL_Rect rect;
	unsigned int shootingFrequency;

};
#endif // !_STEALTHBOMBER_H_




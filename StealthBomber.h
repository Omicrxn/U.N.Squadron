#pragma once
#ifndef _STEALTHBOMBER_H__
#define _STEALTHBOMBER_H__
#include "Enemy.h"
#include "Path.h"
enum SBState {
	SPAWNING, IDLE,SHOOTING
};
class StealthBomber : public Enemy
{
public:
	StealthBomber(int x, int y, bool spawnRight);
	void Update() override;
	void OnCollision(Collider* collider) override;

private:
	Path spawnPath;
	Path normalPath;
	Animation idle;
	Animation shooting;
	SDL_Rect rect;
	unsigned int shootingFrequency;
	unsigned int shootingFrequency2;
	
	SBState state = SPAWNING;
	iPoint idlePosition = { 0, 0};
	
};
#endif // !_STEALTHBOMBER_H_




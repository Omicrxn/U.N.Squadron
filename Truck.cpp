#include "Truck.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Truck::Truck(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	truckAnimation.PushBack({ 87, 6, 78, 21 });

	collider = App->collisions->AddCollider({ position.x , position.y, 78, 21 }, Collider::Type::ENEMY, (Module*)App->enemies);

	scoreGiven = 20000;
	moneyGiven = 5000;
}

void Truck::Update()
{
	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	// After killing all the trucks
	// Money: 5000$
	// Points: 20000
}
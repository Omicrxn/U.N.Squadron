#include "Turret.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SetBulletDirection.h"

Turret::Turret(int x, int y, bool spawnRight) :Enemy(x, y, spawnRight) {

	turretAnimation.PushBack({ 219, 14, 23, 28 });
	turretAnimation.PushBack({ 252, 14, 23, 28 });
	turretAnimation.PushBack({ 287, 13, 23, 28 });
	turretAnimation.speed = 0.05;

	path.PushBack({ 0.0f, 0.0f }, 1000000000, &turretAnimation);

	collider = App->collisions->AddCollider({ position.x, position.y, 23, 28 }, Collider::Type::ENEMY, (Module*)App->enemies);

	scoreGiven = 400;
	moneyGiven = 500;
}

void Turret::Update() {

	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call it to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;
	shootingFrequency++;
	if (shootingFrequency > 120)
	{
		shootingFrequency = 0;

		SetBulletDirection(this);

		App->particles->AddParticle(App->particles->turretMissile, position.x, position.y - 40, Collider::Type::ENEMY_SHOT);
	}
}
#include "GreenFighterPlane.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SetBulletDirection.h"

GreenFighterPlane::GreenFighterPlane(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	fly.PushBack({ 163, 151, 107, 39 });
	currentAnim = &fly;

	// Have the green fighter plane describe a path in the screen
	path.PushBack({ 0.0f, -1.0f }, 50);
	path.PushBack({ 0.0f, 1.0f }, 100);
	path.PushBack({ 0.0f, -1.0f }, 50);

	collider = App->collisions->AddCollider({ position.x, position.y, 107, 39 }, Collider::Type::ENEMY, (Module*)App->enemies);

	scoreGiven = 400;
	moneyGiven = 1200;
}

void GreenFighterPlane::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;
	if (shootingFrequency > 120)
	{
		shootingFrequency = 0;

		SetBulletDirection(this);

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}
}
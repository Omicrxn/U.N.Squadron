#include "StealthBomber.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"
#include "SetBulletDirection.h"

StealthBomber::StealthBomber(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	
	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY, (Module*)App->enemies);
	despawnLeft = false;
	scoreGiven = 100;
	moneyGiven = 300;
}

void StealthBomber::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

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
		App->audio->PlayFx(2, 0);
	}
}
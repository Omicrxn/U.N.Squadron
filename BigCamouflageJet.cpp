#include "BigCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SetBulletDirection.h"

BigCamouflageJet::BigCamouflageJet(int x, int y,bool spawnRight) : Enemy(x, y,spawnRight)
{
	flyRight.PushBack({ 126, 57, 32, 19 });
	flyLeft.PushBack({ 387, 57, 32, 19 });

	// Have the big camouflage jet describe a path in the screen
	if (spawnRight) {
		if (spawnPos.y < SCREEN_HEIGHT / 2) {
			path.PushBack({ -3.0f, 0.0f }, 40, &flyLeft);
			path.PushBack({ -3.0f, 1.0f }, 40, &flyLeft);
			path.PushBack({ -3.0f, 0.0f }, 60, &flyLeft);
		}
		else {
			path.PushBack({ -3.0f, 0.0f }, 40, &flyLeft);
			path.PushBack({ -3.0f, -1.0f }, 40, &flyLeft);
			path.PushBack({ -3.0f, 0.0f }, 60, &flyLeft);
		}
	}
	else {
		if (spawnPos.y < SCREEN_HEIGHT / 2) {
			path.PushBack({ 3.0f, 0.0f }, 40, &flyRight);
			path.PushBack({ 3.0f, 1.0f }, 40, &flyRight);
			path.PushBack({ 3.0f, 0.0f }, 60, &flyRight);
		}
		else {
			path.PushBack({ 3.0f, 0.0f }, 40, &flyRight);
			path.PushBack({ 3.0f, -1.0f }, 40, &flyRight);
			path.PushBack({ 3.0f, 0.0f }, 60, &flyRight);
		}
		
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 19 }, Collider::Type::ENEMY, (Module*)App->enemies);

	scoreGiven = 200;
	moneyGiven = 600;
}

void BigCamouflageJet::Update()
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

		//Playing shooting sound effect
		App->audio->PlayFx(0, 0);
	}
}
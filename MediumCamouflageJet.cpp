#include "MediumCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"
#include "SetBulletDirection.h"

MediumCamouflageJet::MediumCamouflageJet(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	flyLeft.PushBack({ 240, 96, 30, 9 });
	turnRight.PushBack({ 205, 88, 30, 17 });
	turnRight.PushBack({ 173, 82, 30, 23 });
	turnRight.PushBack({ 147, 82, 24, 23 });
	turnRight.PushBack({ 128, 82, 17, 23 });
	turnRight.PushBack({ 113, 82, 10, 23 });
	turnRight.PushBack({ 94, 82, 16, 23 });
	turnRight.PushBack({ 68, 82, 23, 23 });
	turnRight.PushBack({ 36, 81, 30, 24 });
	turnRight.PushBack({ 4, 89, 30, 16 });
	turnRight.speed = 0.1f;
	flyRight.PushBack({ 275, 96, 30, 9 });

	if (spawnPos.y < SCREEN_HEIGHT/2)
	{
		path.PushBack({ -2.0f, 0.0f }, 40, &flyLeft);
		path.PushBack({ -1.5f, 1.0f }, 40, &turnRight);
		path.PushBack({ 3.0f, 0.0f }, 300, &flyRight);
	}
	else
	{
		path.PushBack({ -2.0f, 0.0f }, 40, &flyLeft);
		path.PushBack({ -1.5f, -1.0f }, 40, &turnRight);
		path.PushBack({ 3.0f, 0.0f }, 300, &flyRight);
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY, (Module*)App->enemies);
	despawnLeft = false;
	scoreGiven = 100;
	moneyGiven = 300;
}

void MediumCamouflageJet::Update()
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
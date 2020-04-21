#include "SmallCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"

SmallCamouflageJet::SmallCamouflageJet(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	flyLeft.PushBack({ 245, 112, 26, 8 });
	turnRight.PushBack({ 215, 113, 26, 10 });
	turnRight.PushBack({ 185, 110, 26, 15 });
	turnRight.PushBack({ 163, 110, 19, 15 });
	turnRight.PushBack({ 145, 109, 14, 17 });
	turnRight.PushBack({ 132, 108, 9, 19 });
	turnRight.PushBack({ 119, 109, 11, 17 });
	turnRight.PushBack({ 99, 110, 18, 15 });
	turnRight.PushBack({ 70, 110, 26, 15 });
	turnRight.PushBack({ 40, 12, 26, 10 });
	turnRight.speed = 0.1f;
	flyRight.PushBack({ 274, 112, 26, 8 });

	if (spawnPos.y < SCREEN_HEIGHT / 2)
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

void SmallCamouflageJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;

	if (shootingFrequency > 40)
	{
		shootingFrequency = 0;

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}
}
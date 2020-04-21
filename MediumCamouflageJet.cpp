#include "MediumCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"

MediumCamouflageJet::MediumCamouflageJet(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 240, 96, 30, 9 });
	currentAnim = &fly;

	// Have the medium camouflage jet describe a path in the screen
	if (App->lvl2->numSpawnedEnemies <= 4)
	{
		path.PushBack({ -2.0f, 0.0f }, 20);
		path.PushBack({ -2.0f, 1.0f }, 40);
		path.PushBack({ -2.0f, 0.0f }, 60);
	}
	else
	{
		path.PushBack({ -2.0f, 0.0f }, 20);
		path.PushBack({ -2.0f, -1.0f }, 40);
		path.PushBack({ -2.0f, 0.0f }, 60);
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY, (Module*)App->enemies);

	right = true;
}

void MediumCamouflageJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;
	if (shootingFrequency > 30)
	{
		shootingFrequency = 0;

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}
}
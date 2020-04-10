#include "MediumCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRenderer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"

MediumCamouflageJet::MediumCamouflageJet(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 225,151,29,15 });
	currentAnim = &fly;

	// Have the medium camouflage jet describe a path in the screen

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::ENEMY, (Module*)App->enemies);
}

void MediumCamouflageJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	// Update collider position to medium camouflage jet position
	collider->SetPos(position.x, position.y);

	shootingFrequency++;
	if (shootingFrequency > 50)
	{
		shootingFrequency = 0;

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}
}
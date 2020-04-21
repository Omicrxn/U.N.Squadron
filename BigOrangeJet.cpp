#include "BigOrangeJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"

BigOrangeJet::BigOrangeJet(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	fly.PushBack({ 126, 20, 32, 19 });
	currentAnim = &fly;

	// Have the big orange jet describe a path in the screen
	path.PushBack({ 3.0f, 0.0f }, 20);
	path.PushBack({ 3.0f, -1.0f }, 40);
	path.PushBack({ 3.0f, 0.0f }, 60);

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 19 }, Collider::Type::ENEMY, (Module*)App->enemies);

	//right = true;

	scoreGiven = 200;
	moneyGiven = 600;
}

void BigOrangeJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;
	if (shootingFrequency > 50)
	{
		shootingFrequency = 0;

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}
}
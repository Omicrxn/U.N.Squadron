#include "BlueJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

BlueJet::BlueJet(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 207, 131, 30, 9 });
	currentAnim = &fly;

	// Have the blue jet describe a path in the screen
	path.PushBack({ 0.0f, -1.0f }, 50);
	path.PushBack({ 0.0f, 1.0f }, 100);
	path.PushBack({ 0.0f, -1.0f }, 50);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY, (Module*)App->enemies);

	scoreGiven = 100;
	moneyGiven = 300;
}

void BlueJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call	to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	//shootingFrequency++;
	//if (shootingFrequency > 50)
	//{
	//	shootingFrequency = 0;

	//	App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

	//	// Playing shooting sound effects (if space was pressed)
	//	App->audio->PlayFx(0, 0);
	//}
}
#include "PowerupBlue.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

PowerupBlue::PowerupBlue(int x, int y) : Weapon(x, y)
{
	animPowerBlue.PushBack({ 497, 15, 15, 15 });
	animPowerBlue.PushBack({ 497, 30, 15, 15 });
	animPowerBlue.speed = 0.05f;

	pathBlue.PushBack({ 2.0f, 2.0f }, 60, &animPowerBlue);
	pathBlue.PushBack({ 2.0f, -2.0f }, 60, &animPowerBlue);

	collider = App->collisions->AddCollider({ position.x, position.y, 15, 15 }, Collider::Type::POWERUP, (Module*)App->weapons);
}

void PowerupBlue::Update()
{
	pathBlue.Update();

	position = spawnPos + pathBlue.GetRelativePosition();
	currentAnim = pathBlue.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}

void PowerupBlue::OnCollision(Collider* collider)
{
	if (App->player->level == 1) App->player->pow1 -= 3;
	if (App->player->level == 2) App->player->pow2 -= 3;
	if (App->player->level == 3) App->player->pow3 -= 3;
	if (!App->player->maxPow && App->player->level == 4) App->player->pow4 -= 3;
}
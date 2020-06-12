#include "PowerupOrange.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

PowerupOrange::PowerupOrange(int x, int y) : Weapon(x, y)
{
	animPowerOrange.PushBack({ 497, 0, 15, 15 });
	animPowerOrange.PushBack({ 497, 30, 15, 15 });
	animPowerOrange.speed = 0.05f;

	pathOrange.PushBack({ 2.0f, 2.0f }, 60, &animPowerOrange);
	pathOrange.PushBack({ 2.0f, -2.0f }, 60, &animPowerOrange);

	collider = App->collisions->AddCollider({ position.x, position.y, 15, 15 }, Collider::Type::POWERUP, (Module*)App->weapons);
}

void PowerupOrange::Update()
{
	pathOrange.Update();
	position = spawnPos + pathOrange.GetRelativePosition();
	currentAnim = pathOrange.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}

void PowerupOrange::OnCollision(Collider* collider)
{
	if (App->player->level == 1) --App->player->pow1;
	if (App->player->level == 2) --App->player->pow2;
	if (App->player->level == 3) --App->player->pow3;
	if (!App->player->maxPow && App->player->level == 4) --App->player->pow4;
}
#include "PowerupOrange.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

PowerupOrange::PowerupOrange(int x, int y) : Weapon(x, y)
{
	animPowerOrange.PushBack({ 497, 0, 15, 15 });
	animPowerOrange.PushBack({ 497, 30, 15, 15 });
	animPowerOrange.speed = 0.07f;

	pathOrange.PushBack({ 0.5f, 0.5f }, 60, &animPowerOrange);
	pathOrange.PushBack({ 0.5f, -0.5f }, 60, &animPowerOrange);

	collider = App->collisions->AddCollider({ position.x, position.y, 15, 15 }, Collider::Type::POWERUP_ORANGE, (Module*)App->weapons);
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

}
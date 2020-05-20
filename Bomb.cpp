#include "Bomb.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Bomb::Bomb(int x, int y) : Weapon(x, y)
{
	drop.PushBack({ 132, 207, 15, 5 });

	path.PushBack({ 1.0f, 1.5f }, 100, &drop);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void Bomb::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
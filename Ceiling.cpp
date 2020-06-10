#include "Ceiling.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Ceiling::Ceiling(int x, int y) : Weapon(x, y)
{
	drop.PushBack({ 213, 97, 8, 20 });
	drop.PushBack({ 229, 91, 4, 22 });

	drop.speed = 0.1f;

	path.PushBack({ 1.0f, -0.5f }, 5, &drop);
	path.PushBack({ 1.0f, -1.0f }, 10, &drop);
	path.PushBack({ 1.0f, -2.0f }, 15, &drop);
	path.PushBack({ 1.0f, -3.5f }, 100, &drop);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void Ceiling::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}

void Ceiling::OnCollision(Collider* collider)
{
	
}
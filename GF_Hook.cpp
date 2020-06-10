#include "GF_Hook.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

GF_Hook::GF_Hook(int x, int y) : Weapon(x, y)
{
	hook.PushBack({ 0, 219, 16, 5 });
	hook.PushBack({ 24, 211, 16, 21 });
	hook.PushBack({ 55, 210, 29, 23 });
	hook.speed = 0.08f;
	hook.loop = false;
	path.PushBack({ 1.5f, 0.5f }, 20, &hook);
	path.PushBack({ -1.5f, 0 }, 60, &hook);
	path.PushBack({ -3.5f, 0 }, 30, &hook);
	path.PushBack({ -4.0f, 0 }, 10, &hook);
	path.loop = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}

void GF_Hook::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
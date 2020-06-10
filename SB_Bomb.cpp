#include "SB_Bomb.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
SB_Bomb::SB_Bomb(int x, int y) : Weapon(x, y)
{
	hook.PushBack({ 0, 219, 16, 5 });
	hook.PushBack({ 24, 211, 16, 21 });
	hook.PushBack({ 55, 210, 16, 21 });
	path.PushBack({ 1.0f, 1.5f }, 100, &hook);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void SB_Bomb::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
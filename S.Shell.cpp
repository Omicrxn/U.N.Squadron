#include "S.Shell.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Shell::Shell(int x, int y) : Weapon(x, y)
{
	drop1.PushBack({ 0, 36, 16, 16 });
	drop1.PushBack({ 17, 36, 28, 16 });
	drop1.PushBack({ 46, 30, 48, 26 });
	drop1.PushBack({ 95, 29, 80, 26 });
	drop2.PushBack({ 176, 34, 77, 14 });

	drop1.speed = 0.05f;

	path.PushBack({ 5.0f, 0.0f }, 10, &drop1);
	path.PushBack({ 5.0f, 0.0f }, 90, &drop2);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void Shell::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
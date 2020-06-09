#include "Falcon.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Falcon::Falcon(int x, int y) : Weapon(x, y)
{
	drop.PushBack({ 132, 149, 16, 7 });

	if (App->player->GetPlayerPosition().y < SCREEN_HEIGHT / 4)
	{
		path.PushBack({ 1.0f, 1.5f }, 100, &drop);
	}
	else if (App->player->GetPlayerPosition().y > SCREEN_HEIGHT / 4)
	{
		path.PushBack({ 1.0f, 1.5f }, 75, &drop);
	}
	else if (App->player->GetPlayerPosition().y > SCREEN_HEIGHT / 2)
	{
		path.PushBack({ 1.0f, 1.5f }, 50, &drop);
	}
	else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4)))
	{
		path.PushBack({ 1.0f, 1.5f }, 25, &drop);
	}
	
	path.PushBack({ 2.0f, 1.5f }, 5, &drop);
	path.PushBack({ 3.0f, 1.0f }, 10, &drop);
	path.PushBack({ 4.0f, 0.0f }, 15, &drop);
	path.PushBack({ 5.0f, 0.0f }, 60, &drop);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void Falcon::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
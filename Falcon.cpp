#include "Falcon.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Falcon::Falcon(int x, int y) : Weapon(x, y)
{
	drop1.PushBack({ 0, 10, 14, 6 });
	drop1.PushBack({ 15, 9, 14, 8 });
	drop1.PushBack({ 30, 10, 14, 16 });
	drop1.PushBack({ 45, 11, 14, 4 });
	drop2.PushBack({ 60, 6, 29, 14 });
	drop3.PushBack({ 90, 9, 29, 8 });
	drop3.PushBack({ 120, 9, 33, 8 });

	drop1.speed = 0.1f;
	drop2.speed = 0.1f;
	drop3.speed = 0.1f;

	if (App->player->GetPlayerPosition().y < (SCREEN_HEIGHT / 6))
	{
		path.PushBack({ 1.0f, 2.0f }, 80, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 10, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}
	else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT / 6) && App->player->GetPlayerPosition().y < ((SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 6 )))
	{
		path.PushBack({ 1.0f, 2.0f }, 50, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 10, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}
	else if (App->player->GetPlayerPosition().y > ((SCREEN_HEIGHT / 6) * 2) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT / 2))
	{
		path.PushBack({ 1.0f, 2.0f }, 30, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 10, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}
	else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT / 2) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT - ((SCREEN_HEIGHT / 6) * 2)))
	{
		path.PushBack({ 1.0f, 2.0f }, 10, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 10, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}
	else if (App->player->GetPlayerPosition().y > ((SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 6)) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)))
	{
		path.PushBack({ 1.0f, 2.0f }, 5, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 2.5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 5, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}
	/*else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)))
	{
		path.PushBack({ 1.0f, 1.5f }, 5, &drop);
		path.PushBack({ 2.0f, 1.5f }, 5, &drop);
		path.PushBack({ 3.0f, 1.0f }, 10, &drop);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop);
	}*/
	else
	{
		path.PushBack({ 1.0f, 2.0f }, 10, &drop1);
		path.PushBack({ 2.0f, 2.0f }, 5, &drop2);
		path.PushBack({ 3.0f, 1.5f }, 10, &drop2);
		path.PushBack({ 4.0f, 0.0f }, 15, &drop3);
		path.PushBack({ 5.0f, 0.0f }, 60, &drop3);
	}

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

void Falcon::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->falconExplosion, position.x, position.y);
	App->audio->PlayFx(3, 0);
}
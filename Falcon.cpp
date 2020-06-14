#include "Falcon.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

Falcon::Falcon(int x, int y) : Weapon(x, y)
{
	dropRight1.PushBack({ 0, 10, 14, 6 });
	dropRight1.PushBack({ 15, 9, 14, 8 });
	dropRight1.PushBack({ 30, 10, 14, 16 });
	dropRight1.PushBack({ 45, 11, 14, 4 });
	dropRight2.PushBack({ 60, 6, 29, 14 });
	dropRight3.PushBack({ 90, 9, 29, 8 });
	dropRight3.PushBack({ 120, 9, 33, 8 });

	dropLeft1.PushBack({ 498, 243, 14, 6 });
	dropLeft1.PushBack({ 483, 242, 14, 8 });
	dropLeft1.PushBack({ 468, 243, 14, 16 });
	dropLeft1.PushBack({ 453, 244, 14, 4 });
	dropLeft2.PushBack({ 423, 239, 29, 14 });
	dropLeft3.PushBack({ 393, 242, 29, 8 });
	dropLeft3.PushBack({ 359, 242, 33, 8 });

	dropRight1.speed = 0.1f;
	dropRight2.speed = 0.1f;
	dropRight3.speed = 0.1f;

	dropLeft1.speed = 0.1f;
	dropLeft2.speed = 0.1f;
	dropLeft3.speed = 0.1f;

	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ 1.0f + App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion}, 5, &dropRight1);
					path.PushBack({ 2.0f + App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 2.5, &dropRight2);
					path.PushBack({ 3.0f + App->lvl1->cameraSpeed, 1.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropRight2);
					path.PushBack({ 4.0f + App->lvl1->cameraSpeed, 0.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropRight3);
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 0.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 60, &dropRight3);
				}
				else if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ 1.0f + App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropRight1);
					path.PushBack({ 2.0f + App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 2.5, &dropRight2);
					path.PushBack({ 3.0f + App->lvl1->cameraSpeed, 1.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropRight2);
					path.PushBack({ 4.0f + App->lvl1->cameraSpeed, 0.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropRight3);
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 0.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 60, &dropRight3);
				}
			}
			else
			{
				path.PushBack({ 1.0f + App->lvl1->cameraSpeed, 2.0f }, 5, &dropRight1);
				path.PushBack({ 2.0f + App->lvl1->cameraSpeed, 2.0f }, 2.5, &dropRight2);
				path.PushBack({ 3.0f + App->lvl1->cameraSpeed, 1.5f }, 5, &dropRight2);
				path.PushBack({ 4.0f + App->lvl1->cameraSpeed, 0.0f }, 15, &dropRight3);
				path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 0.0f }, 60, &dropRight3);
			}
		}
		else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ -1.0f - App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropLeft1);
					path.PushBack({ -2.0f - App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 2.5, &dropLeft2);
					path.PushBack({ -3.0f - App->lvl1->cameraSpeed, 1.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropLeft2);
					path.PushBack({ -4.0f - App->lvl1->cameraSpeed, 0.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropLeft3);
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 0.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 60, &dropLeft3);
				}
				else if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ -1.0f - App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropLeft1);
					path.PushBack({ -2.0f - App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 2.5, &dropLeft2);
					path.PushBack({ -3.0f - App->lvl1->cameraSpeed, 1.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &dropLeft2);
					path.PushBack({ -4.0f - App->lvl1->cameraSpeed, 0.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropLeft3);
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 0.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 60, &dropLeft3);
				}
			}
			else
			{
				path.PushBack({ -1.0f - App->lvl1->cameraSpeed, 2.0f }, 5, &dropLeft1);
				path.PushBack({ -2.0f - App->lvl1->cameraSpeed, 2.0f }, 2.5, &dropLeft2);
				path.PushBack({ -3.0f - App->lvl1->cameraSpeed, 1.5f }, 5, &dropLeft2);
				path.PushBack({ -4.0f - App->lvl1->cameraSpeed, 0.0f }, 15, &dropLeft3);
				path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 0.0f }, 60, &dropLeft3);
			}
		}
	}
	else if (App->lvl2->IsEnabled())
	{
		if (App->player->GetPlayerPosition().y < (SCREEN_HEIGHT / 6))
		{
			path.PushBack({ 1.0f, 2.0f }, 80, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 10, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
		}
		else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT / 6) && App->player->GetPlayerPosition().y < ((SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 6)))
		{
			path.PushBack({ 1.0f, 2.0f }, 50, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 10, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
		}
		else if (App->player->GetPlayerPosition().y > ((SCREEN_HEIGHT / 6) * 2) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT / 2))
		{
			path.PushBack({ 1.0f, 2.0f }, 30, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 10, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
		}
		else if (App->player->GetPlayerPosition().y > (SCREEN_HEIGHT / 2) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT - ((SCREEN_HEIGHT / 6) * 2)))
		{
			path.PushBack({ 1.0f, 2.0f }, 10, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 10, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
		}
		else if (App->player->GetPlayerPosition().y > ((SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 6)) && App->player->GetPlayerPosition().y < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)))
		{
			path.PushBack({ 1.0f, 2.0f }, 5, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 2.5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 5, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
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
			path.PushBack({ 1.0f, 2.0f }, 10, &dropRight1);
			path.PushBack({ 2.0f, 2.0f }, 5, &dropRight2);
			path.PushBack({ 3.0f, 1.5f }, 10, &dropRight2);
			path.PushBack({ 4.0f, 0.0f }, 15, &dropRight3);
			path.PushBack({ 5.0f, 0.0f }, 60, &dropRight3);
		}
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::WEAPON, (Module*)App->weapons);
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
	App->audio->PlayFx(destroyedFx, 0);
}
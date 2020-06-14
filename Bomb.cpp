#include "Bomb.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

Bomb::Bomb(int x, int y) : Weapon(x, y)
{
	dropLeft1.PushBack({ 499, 313, 13, 10 });
	dropLeft2.PushBack({ 485, 309, 13, 14 });

	dropRight1.PushBack({ 0, 80, 13, 10 });
	dropRight2.PushBack({ 14, 76, 13, 14 });

	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropRight1);
					path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 85, &dropRight2);
				}
				else if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropRight1);
					path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 85, &dropRight2);
				}
			}
			else
			{
				path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f }, 15, &dropRight1);
				path.PushBack({ 1.5f + App->lvl1->cameraSpeed, 2.0f }, 85, &dropRight2);
			}
		}
		else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropLeft1);
					path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 85, &dropLeft2);
				}
				else if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &dropLeft1);
					path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 85, &dropLeft2);
				}
			}
			else
			{
				path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f }, 15, &dropLeft1);
				path.PushBack({ -1.5f - App->lvl1->cameraSpeed, 2.0f }, 85, &dropLeft2);
			}
		}
	}
	else if (App->lvl2->IsEnabled())
	{
		path.PushBack({ 1.5f, 2.0f }, 15, &dropRight1);
		path.PushBack({ 1.5f, 2.0f }, 85, &dropRight2);
	}
	

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::WEAPON, (Module*)App->weapons);
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

void Bomb::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->bombExplosion, position.x, position.y);
	App->audio->PlayFx(destroyedFx, 0);
}
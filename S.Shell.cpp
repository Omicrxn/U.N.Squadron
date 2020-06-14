#include "S.Shell.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

Shell::Shell(int x, int y) : Weapon(x, y)
{
	dropRight1.PushBack({ 0, 36, 16, 16 });
	dropRight1.PushBack({ 17, 36, 28, 16 });
	dropRight1.PushBack({ 46, 30, 48, 26 });
	dropRight1.PushBack({ 95, 29, 80, 26 });
	dropRight2.PushBack({ 176, 34, 77, 14 });

	dropLeft1.PushBack({ 496, 269, 16, 16 });
	dropLeft1.PushBack({ 467, 269, 28, 16 });
	dropLeft1.PushBack({ 418, 263, 48, 26 });
	dropLeft1.PushBack({ 337, 263, 80, 26 });
	dropLeft2.PushBack({ 259, 267, 77, 14 });

	dropRight1.speed = 0.05f;
	dropLeft1.speed = 0.05f;

	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, -1 + (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &dropRight1);
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, -1 + (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 90, &dropRight2);
				}
				else if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 1 -(float)App->lvl1->cameraSpeed / App->lvl1->proportion}, 10, &dropRight1);
					path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 1 -(float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 90, &dropRight2);
				}
			}
			else
			{
				path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 0.0f }, 10, &dropRight1);
				path.PushBack({ 5.0f + App->lvl1->cameraSpeed, 0.0f }, 90, &dropRight2);
			}
		}
		else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, -1 + (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &dropLeft1);
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, -1 + (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 90, &dropLeft2);
				}
				else if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 1 - (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &dropLeft1);
					path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 1 - (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 90, &dropLeft2);
				}
			}
			else
			{
				path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 0.0f }, 10, &dropLeft1);
				path.PushBack({ -5.0f - App->lvl1->cameraSpeed, 0.0f }, 90, &dropLeft2);
			}
		}
	}
	else if (App->lvl2->IsEnabled())
	{
		path.PushBack({ 5.0f, 0.0f }, 10, &dropRight1);
		path.PushBack({ 5.0f, 0.0f }, 90, &dropRight2);
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::WEAPON_SHELL, (Module*)App->weapons);
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
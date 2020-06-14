#include "Level1Gun.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

Level1Gun::Level1Gun(int x, int y) : Weapon(x, y)
{
	dropRight.PushBack({ 434, 8, 13, 3 });
	dropLeft.PushBack({ 477, 54, 13, 3 });

	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ 7.0f + App->lvl1->cameraSpeed,  (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 180, &dropRight);
				}
				else if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ 7.0f + App->lvl1->cameraSpeed,  -(float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 180, &dropRight);
				}
			}
			else
			{
				path.PushBack({ 7.0f + App->lvl1->cameraSpeed, 0.0f }, 180, &dropRight);
			}
		}
		else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ -7.0f - App->lvl1->cameraSpeed,  (float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 180, &dropLeft);
				}
				else if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ -7.0f - App->lvl1->cameraSpeed, -(float)App->lvl1->cameraSpeed / App->lvl1->proportion }, 180, &dropLeft);
				}
			}
			else
			{
				path.PushBack({ -7.0f - App->lvl1->cameraSpeed, 0.0f }, 180, &dropLeft);
			}
		}
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 13, 3 }, Collider::Type::PLAYER_SHOT, (Module*)App->weapons);
}

void Level1Gun::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
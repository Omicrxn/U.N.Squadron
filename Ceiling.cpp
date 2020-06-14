#include "Ceiling.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

Ceiling::Ceiling(int x, int y) : Weapon(x, y)
{
	drop.PushBack({ 213, 97, 8, 20 });
	drop.PushBack({ 229, 91, 4, 22 });

	drop.speed = 0.1f;

	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ (float)App->lvl1->cameraSpeed, -0.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -1.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -3.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 100, &drop);
				}
				else if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ (float)App->lvl1->cameraSpeed, -0.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -1.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &drop);
					path.PushBack({ (float)App->lvl1->cameraSpeed, -3.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 100, &drop);
				}
			}
			else
			{
				path.PushBack({ (float)App->lvl1->cameraSpeed, -0.5f }, 5, &drop);
				path.PushBack({ (float)App->lvl1->cameraSpeed, -1.0f }, 10, &drop);
				path.PushBack({ (float)App->lvl1->cameraSpeed, -2.0f }, 15, &drop);
				path.PushBack({ (float)App->lvl1->cameraSpeed, -3.5f }, 100, &drop);
			}
		}
		else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
		{
			if (App->lvl1->currentPosition == App->lvl1->LEFTUP || App->lvl1->currentPosition == App->lvl1->RIGHTUP)
			{
				if (App->lvl1->currentPosition == App->lvl1->RIGHTUP)
				{
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -0.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -1.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -2.0f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -3.5f + App->lvl1->cameraSpeed / App->lvl1->proportion }, 100, &drop);
				}
				if (App->lvl1->currentPosition == App->lvl1->LEFTUP)
				{
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -0.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 5, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -1.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 10, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -2.0f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 15, &drop);
					path.PushBack({ -(float)App->lvl1->cameraSpeed, -3.5f - App->lvl1->cameraSpeed / App->lvl1->proportion }, 100, &drop);
				}
			}
			else
			{
				path.PushBack({ -(float)App->lvl1->cameraSpeed, -0.5f }, 5, &drop);
				path.PushBack({ -(float)App->lvl1->cameraSpeed, -1.0f }, 10, &drop);
				path.PushBack({ -(float)App->lvl1->cameraSpeed, -2.0f }, 15, &drop);
				path.PushBack({ -(float)App->lvl1->cameraSpeed, -3.5f }, 100, &drop);
			}
		}
			
	}
	else if (App->lvl2->IsEnabled())
	{
		path.PushBack({ 1.0f, -0.5f }, 5, &drop);
		path.PushBack({ 1.0f, -1.0f }, 10, &drop);
		path.PushBack({ 1.0f, -2.0f }, 15, &drop);
		path.PushBack({ 1.0f, -3.5f }, 100, &drop);
	}

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::WEAPON, (Module*)App->weapons);
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
	App->audio->PlayFx(destroyedFx, 0);
}
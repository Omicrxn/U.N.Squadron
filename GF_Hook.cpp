#include "GF_Hook.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "GreenFighterPlane.h"
#include "ModulePlayer.h"

GF_Hook::GF_Hook(int x, int y,unsigned int number) : Weapon(x, y)
{
	hookNum = number;
	hook.PushBack({ 0, 219, 16, 5 });
	hook.PushBack({ 24, 211, 16, 21 });
	hook.PushBack({ 55, 210, 29, 23 });
	hook.speed = 0.08f;
	hook.loop = false;
	
	path0.PushBack({ 1.5f, 0.5f }, 20, &hook);
	path0.PushBack({ -1.5f, 0 }, 60, &hook);
	path0.PushBack({ -3.5f, 0 }, 30, &hook);
	path0.PushBack({ -4.0f, 0 }, 10, &hook);
	path0.loop = false;
	path1.PushBack({ 1.5f, -0.5f }, 20, &hook);
	path1.PushBack({ -1.5f, 0 }, 60, &hook);
	path1.PushBack({ -3.5f, 0 }, 30, &hook);
	path1.PushBack({ -4.0f, 0 }, 10, &hook);
	path1.loop = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 29, 23 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}

void GF_Hook::Update()
{
	switch (hookNum)
	{
	case 0:
		path0.Update();
		position = spawnPos + path0.GetRelativePosition();
		currentAnim = path0.GetCurrentAnimation();
		break;
	case 1:
		path1.Update();
		position = spawnPos + path1.GetRelativePosition();
		currentAnim = path1.GetCurrentAnimation();
		break;
	default:
		break;
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}
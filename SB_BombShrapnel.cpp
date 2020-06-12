#include "SB_BombShrapnel.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
SB_BombShrapnel::SB_BombShrapnel(int x, int y,unsigned int number) : Weapon(x, y)
{
	shrapnelNum = number;
	shrapnel.PushBack({ 41,175,12,4 });
	//PATH0 HORIZONTAL-LEFT
	path0.PushBack({ -0.5f, 0.0f }, 60, &shrapnel);
	path0.loop = false;
	//PATH1 UPPER-LEFT-DIAGONAL
	path1.PushBack({ -0.5f, -1.5f }, 60, &shrapnel);
	path1.loop = false;
	//PATH2 UPPER
	path2.PushBack({ 1.0f, -1.5f }, 60, &shrapnel);
	path2.loop = false;
	//PATH3 UPPER-RIGHT-DIAGONAL
	path3.PushBack({ -1.5f, -1.5f }, 60, &shrapnel);
	path3.loop = false;
	//PATH4 HORIZONTAL-RIGHT
	path4.PushBack({ 1.5f, 0.0f }, 60, &shrapnel);
	path4.loop = false;
	//PATH5 LOWER-RIGHT-DIAGONAL
	path5.PushBack({ 1.5f, 1.5f }, 60, &shrapnel);
	path5.loop = false;
	//PATH6 LOWER
	path6.PushBack({ 1.0f,1.5f }, 60, &shrapnel);
	path6.loop = false;
	//PATH7  LOWER-LEFT-DIAGONAL
	path7.PushBack({ -0.5f, -0.5f }, 60, &shrapnel);
	path7.loop = false;


	collider = App->collisions->AddCollider({ position.x, position.y, 12, 4 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}

void SB_BombShrapnel::Update()
{
	switch (shrapnelNum)
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
	case 2:
		path2.Update();
		position = spawnPos + path2.GetRelativePosition();
		currentAnim = path2.GetCurrentAnimation();
		break;
	case 3:
		path3.Update();
		position = spawnPos + path3.GetRelativePosition();
		currentAnim = path3.GetCurrentAnimation();
		break;
	case 4:
		path4.Update();
		position = spawnPos + path4.GetRelativePosition();
		currentAnim = path4.GetCurrentAnimation();
		break;
	case 5:
		path5.Update();
		position = spawnPos + path5.GetRelativePosition();
		currentAnim = path5.GetCurrentAnimation();
		break;
	case 6:
		path6.Update();
		position = spawnPos + path6.GetRelativePosition();
		currentAnim = path6.GetCurrentAnimation();
		break;
	case 7:
		path7.Update();
		position = spawnPos + path7.GetRelativePosition();
		currentAnim = path7.GetCurrentAnimation();
		break;
	default:
		break;
	}
	

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
	
}
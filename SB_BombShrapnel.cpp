#include "SB_BombShrapnel.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
SB_BombShrapnel::SB_BombShrapnel(int x, int y,unsigned int number) : Weapon(x, y)
{
	shrapnelNum = number;
	shrapnel0.PushBack({ 41,175,10,4 });
	shrapnel1.PushBack({ 52,175,6,8 });
	shrapnel2.PushBack({ 59,175,4,12 });
	shrapnel3.PushBack({ 64,175,6,8 });
	shrapnel4.PushBack({ 71,175,10,4 });
	shrapnel5.PushBack({ 82,175,6,8 });
	shrapnel6.PushBack({ 89,175,4,12 });
	shrapnel7.PushBack({ 94,175,6,8 });
	//PATH0 HORIZONTAL-LEFT
	path0.PushBack({ -0.5f, 0.0f }, 60, &shrapnel0);
	path0.loop = false;
	//PATH1 UPPER-LEFT-DIAGONAL
	path1.PushBack({ -0.0f, -1.0f }, 60, &shrapnel1);
	path1.loop = false;
	//PATH2 UPPER
	path2.PushBack({ 1.0f, -1.5f }, 60, &shrapnel2);
	path2.loop = false;
	//PATH3 UPPER-RIGHT-DIAGONAL
	path3.PushBack({ 2.0f, -1.0f }, 60, &shrapnel3);
	path3.loop = false;
	//PATH4 HORIZONTAL-RIGHT
	path4.PushBack({ 2.5f, 0.0f }, 60, &shrapnel4);
	path4.loop = false;
	//PATH5 LOWER-RIGHT-DIAGONAL
	path5.PushBack({ 2.0f, 1.0f }, 60, &shrapnel5);
	path5.loop = false;
	//PATH6 LOWER
	path6.PushBack({ 1.0f,1.5f }, 60, &shrapnel6);
	path6.loop = false;
	//PATH7  LOWER-LEFT-DIAGONAL
	path7.PushBack({ 0.0f, 1.0f }, 60, &shrapnel7);
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
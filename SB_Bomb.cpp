#include "SB_Bomb.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleWeapons.h"
SB_Bomb::SB_Bomb(int x, int y,unsigned int number) : Weapon(x, y)
{
	bombNum = number;
	bomb.PushBack({ 0, 176, 9, 10 });
	bomb.PushBack({ 14, 176, 9, 10 });
	bomb.PushBack({ 27, 176, 11, 12 });
	bomb.speed = 0.1f;
	path0.PushBack({ 0.3f, -1.1f }, 40, &bomb);
	path0.PushBack({ 0.1f, -0.0f }, 20, &bomb);
	path0.PushBack({ 0.1f, 0.5f }, 20, &bomb);
	path0.PushBack({ 0.1f, 0.8f }, 10, &bomb);
	path0.PushBack({1.0f, 0.0f }, 10, &bomb);
	path0.loop = false;
	path1.PushBack({ 0.1f, -1.1f }, 40, &bomb);
	path1.PushBack({ 0.1f, -0.0f }, 20, &bomb);
	path1.PushBack({ 0.1f, 0.5f }, 20, &bomb);
	path1.PushBack({ 0.1f, 0.8f }, 40, &bomb);
	path1.PushBack({ 1.0f, 0.0f }, 10, &bomb);
	path1.loop = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 11, 12 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}
void SB_Bomb::Explosion(Path* path) {
	if (path->Finished())
	{
		App->particles->AddParticle(App->particles->sbBombExplosion, position.x - 10, position.y - 6, Collider::Type::ENEMY_SHOT, 0);
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 0);//h-l
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 1);//d-u-l
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 2);//u
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 3);//d-u-r
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 4);//h-r
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 5);//d-l-r
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 6);
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_SHRAPNEL, position.x, position.y, 7);
		this->pendingToDelete = true;
	}
	
}
void SB_Bomb::Update()
{
	switch(bombNum)
	{
	case 0:
		path0.Update();
		position = spawnPos + path0.GetRelativePosition();
		currentAnim = path0.GetCurrentAnimation();
		Explosion(&path0);
		break;
	case 1:

		path1.Update();
		position = spawnPos + path1.GetRelativePosition();
		currentAnim = path1.GetCurrentAnimation();
		Explosion(&path1);
		break;
	default:
		break;
	}
	

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
	
}
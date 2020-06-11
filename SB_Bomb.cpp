#include "SB_Bomb.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleWeapons.h"
SB_Bomb::SB_Bomb(int x, int y) : Weapon(x, y)
{
	bomb.PushBack({ 0, 176, 9, 10 });
	bomb.PushBack({ 14, 176, 9, 10 });
	bomb.PushBack({ 27, 176, 11, 12 });
	bomb.speed = 0.1f;
	path.PushBack({ 0.1f, -1.1f }, 60, &bomb);
	path.PushBack({1.0f, 0.0f }, 10, &bomb);
	path.loop = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 11, 12 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}

void SB_Bomb::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
	if (path.Finished()) {

		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y, Collider::Type::ENEMY_SHOT,0);
		App->weapons->SpawnShrapnel(position.x,position.y,0);
		App->weapons->SpawnShrapnel(position.x,position.y,1);
		App->weapons->SpawnShrapnel(position.x,position.y,2);
		App->weapons->SpawnShrapnel(position.x,position.y,3);
		App->weapons->SpawnShrapnel(position.x,position.y,4);
		App->weapons->SpawnShrapnel(position.x,position.y,5);
		App->weapons->SpawnShrapnel(position.x,position.y,6);
		App->weapons->SpawnShrapnel(position.x,position.y,7);
		this->pendingToDelete = true;
	}
}
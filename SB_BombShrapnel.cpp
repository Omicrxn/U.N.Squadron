#include "SB_BombShrapnel.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
SB_BombShrapnel::SB_BombShrapnel(int x, int y) : Weapon(x, y)
{
	shrapnel.PushBack({ 41,175,12,4 });
	//PATH1 HORIZONTAL-LEFT
	path1.PushBack({ 0.1f, -1.1f }, 60, &shrapnel);
	path1.loop = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY_SHOT, (Module*)App->weapons);
}

void SB_BombShrapnel::Update()
{
	//path.Update();
	//position = spawnPos + path.GetRelativePosition();
	//currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
	if (path1.Finished()) {

		App->particles->AddParticle(App->particles->bullet1, position.x, position.y, Collider::Type::ENEMY_SHOT, 5);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y, Collider::Type::ENEMY_SHOT, 0);
		this->pendingToDelete = true;
	}
}
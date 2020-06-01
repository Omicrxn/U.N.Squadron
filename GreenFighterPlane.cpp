#include "GreenFighterPlane.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SetBulletDirection.h"

GreenFighterPlane::GreenFighterPlane(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	//animations
	idle.PushBack({ 275,151,107,39 });
	//collision
	collider = App->collisions->AddCollider({ position.x, position.y, 139, 79 }, Collider::Type::BOSS, (Module*)App->enemies);
	//paths
	spawnPath.PushBack({ 1.12f,0.65f }, 240, &idle);
	spawnPath.PushBack({ 1.0f,0.0f }, 10, &idle);
	
	spawnPath.loop = false;
	normalPath.PushBack({ 1.13f,-0.7f }, 180, &idle);//up-right
	normalPath.PushBack({ 1.0f,0.0f }, 10, &idle);
	normalPath.PushBack({ 1.01f,0.7f }, 180, &idle);//down-right
	normalPath.PushBack({ 1.0f,0.0f }, 10, &idle);
	normalPath.PushBack({ 1.08f,-0.7f }, 180, &idle);//up-right
	normalPath.PushBack({ 1.0f,0.0f }, 10, &idle);
	normalPath.PushBack({ 0.82f,0.7f }, 180, &idle);//down-left
	normalPath.PushBack({ 1.0f,0.0f }, 10, &idle);
	normalPath.PushBack({ 1.13f,-0.7f }, 275, &idle);//up-right and despawn
	normalPath.PushBack({ 2.0f,-0.0f }, 180, &idle);//up-right and despawn
	normalPath.loop = false;
	//other var
	despawnLeft = false;
	scoreGiven = 400;
	moneyGiven = 1200;
}

void GreenFighterPlane::Update()
{
	switch (state)
	{
	case gSPAWNING:
		spawnPath.Update();
		if (spawnPath.Finished())
		{
			state = gIDLE;
		}
		if (currentAnim != spawnPath.GetCurrentAnimation()) {
			currentAnim = spawnPath.GetCurrentAnimation();
		}
		position = spawnPos + spawnPath.GetRelativePosition();
		idlePosition = position;
		break;
	case gIDLE:

		normalPath.Update();

		position = idlePosition + normalPath.GetRelativePosition();
		if (currentAnim != normalPath.GetCurrentAnimation()) {
			currentAnim = normalPath.GetCurrentAnimation();
		}
		break;
	default:
		break;
	}

	Enemy::Update();

	shootingFrequency++;
	if (state == gIDLE && shootingFrequency > 60)
	{
		shootingFrequency = 0;

		SetBulletDirection(this);

		App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(2, 0);
	}
}
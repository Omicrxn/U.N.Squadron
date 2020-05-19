#include "StealthBomber.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevel2.h"
#include "SetBulletDirection.h"

StealthBomber::StealthBomber(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	idle.PushBack({ 3,6,139,79 });
	shooting.PushBack({ 176, 6, 139, 79 });
	shooting.PushBack({ 291, 6, 139, 79 });
	shooting.PushBack({ 3, 88, 139, 79 });
	shooting.PushBack({ 150, 88, 139, 79 });
	shooting.speed = 0.05f;
	shooting.loop = false;
	//collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::ENEMY, (Module*)App->enemies);
	spawnPath.PushBack({ 1.5f,-0.5f }, 180, &idle);
	spawnPath.PushBack({ 1.0f,-0.0f }, 1, &idle);
	spawnPath.loop = false;
	normalPath.PushBack({ 2.0f, -0.5f }, 180, &idle);
	normalPath.PushBack({ -1.5f,0.5f }, 180, &idle);
	normalPath.loop = true;
	despawnLeft = false;
	scoreGiven = 100;
	moneyGiven = 300;
}

void StealthBomber::Update()
{
	spawnPath.Update();
	normalPath.Update();
	position = spawnPos + spawnPath.GetRelativePosition();
	if (currentAnim != spawnPath.GetCurrentAnimation()) {
		currentAnim = spawnPath.GetCurrentAnimation();
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();

	shootingFrequency++;
	if (shootingFrequency > 120)
	{
		shootingFrequency = 0;

		//SetBulletDirection(this);
		/*if (currentAnim != &shooting) {
			currentAnim = &shooting;
			shooting.Reset();
		}*/
		App->particles->AddParticle(App->particles->sbFirecannon, position.x + 7, position.y+32, Collider::Type::ENEMY_SHOT,0);
		App->particles->AddParticle(App->particles->sbFirecannon, position.x + 7, position.y+40, Collider::Type::ENEMY_SHOT,120);
		App->particles->AddParticle(App->particles->sbFirecannon, position.x + 7, position.y+45, Collider::Type::ENEMY_SHOT,200);
		App->particles->AddParticle(App->particles->sbFirecannon, position.x + 7, position.y+60, Collider::Type::ENEMY_SHOT,300);
		App->particles->AddParticle(App->particles->sbFirecannon, position.x + 7, position.y+72, Collider::Type::ENEMY_SHOT,400);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(2, 0);
	}
}
#include "StealthBomber.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleLevel2.h"
#include "SetBulletDirection.h"
#include "ModuleDebugInfo.h"
#include "ModuleWeapons.h"
#include "ModulePlayer.h"

StealthBomber::StealthBomber(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	//animations
	idle.PushBack({ 3,6,139,79 });
	shooting.PushBack({ 146, 6, 139, 79 });
	shooting.PushBack({ 291, 6, 139, 79 });
	shooting.PushBack({ 3, 88, 139, 79 });
	shooting.PushBack({ 150, 88, 139, 79 });
	shooting.speed = 0.08f;
	shooting.loop = false;
	//collision
	collider = App->collisions->AddCollider({ position.x, position.y, 139, 79 }, Collider::Type::BOSS, (Module*)App->enemies);
	//paths
	spawnPath.PushBack({ 1.43f,-0.2f }, 600, &idle);
	spawnPath.PushBack({ 1.0-0.15f,-0.25f }, 180, &idle);
	spawnPath.loop = false;
	normalPath.PushBack({ 0.9f,0.3f }, 180, &idle);
	normalPath.PushBack({ 1.1f, -0.3f }, 180, &idle);
	normalPath.loop = true;
	//other var
	despawnLeft = false;
	scoreGiven = 100;
	moneyGiven = 300;
	health = 140;
	currentAnim = spawnPath.GetCurrentAnimation();

}

void StealthBomber::Update()
{
	
	if (damaged)
	{
		currentTime++;
		if (currentTime == 5 )
		{
			SDL_SetTextureColorMod(App->enemies->sbTexture, 255, 255, 255);
			damaged = false;
			currentTime = 0;
		}
	}
	switch (state)
	{
	case SPAWNING:
		spawnPath.Update();
		if (spawnPath.Finished())
		{
			state = IDLE;
		}
		if (currentAnim != spawnPath.GetCurrentAnimation()) {
			currentAnim = spawnPath.GetCurrentAnimation();
		}
		position = spawnPos + spawnPath.GetRelativePosition();
		idlePosition = position;
		break;
	case IDLE:
		shootingFrequency++;
		shootingFrequency2++;
		normalPath.Update();

		position = idlePosition + normalPath.GetRelativePosition();
		if (shooting.Finished() && currentAnim != normalPath.GetCurrentAnimation()) {
			currentAnim = normalPath.GetCurrentAnimation();

		}
		break;
	default:
		break;
	}
	
	if (shootingFrequency == 190 && state == IDLE)
	{

		shootingFrequency = 0;
		LOG("Shooting particle");
		App->particles->AddParticle(App->particles->sbSparks, position.x-1, position.y + 24, Collider::Type::ENEMY_SHOT, 0);
		App->particles->AddParticle(App->particles->sbFirecannon, position.x-1-24, position.y + 24, Collider::Type::ENEMY_SHOT, 5);
		
		App->audio->PlayFx(App->enemies->shootBossFx, 0);
	}
	if (shootingFrequency2 == 180 && state == IDLE) {
		shootingFrequency2 = 0;
		if (currentAnim != &shooting) {
			currentAnim = &shooting;
			shooting.Reset();
		}

		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_BOMB, position.x+65, position.y+18,0);
		App->weapons->SpawnWeapon(WEAPON_TYPE::SB_BOMB, position.x+65, position.y+46,1);

	}
	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}

void StealthBomber::OnCollision(Collider* collider) {
	damaged = true;
	SDL_SetTextureColorMod(App->enemies->sbTexture, 250, 200, 75);
	if (health > 1) {
		if (collider->type == Collider::Type::PLAYER_SHOT) {
			if (App->player->level == 1) health--;
			else if (App->player->level == 2) health -= 2;
			else if (App->player->level == 3) health -= 3;
			else if (App->player->level == 4) health -= 4;
			else if (App->player->level == 5) health -= 5;
		}
		else if (collider->type == Collider::Type::WEAPON || collider->type == Collider::Type::WEAPON_SHELL) {
			health -= 7;
		}
	}
	else {
		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x + 10, position.y - 4);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x - 10, position.y + 4);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x + 15, position.y - 4);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x - 15, position.y + 4);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x + 20, position.y);
		App->particles->AddParticle(App->particles->enemyExplosion, position.x - 20, position.y);

		App->audio->PlayFx(destroyedFx, 0);

		App->player->score += scoreGiven;

		if (!App->debugInfo->maxMoney) {
			App->player->money += moneyGiven;
		}
		this->SetToDelete();
	}
}
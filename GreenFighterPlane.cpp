#include "GreenFighterPlane.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SetBulletDirection.h"
#include "ModuleWeapons.h"
#include "ModuleDebugInfo.h"
#include "ModuleEnemies.h"

GreenFighterPlane::GreenFighterPlane(int x, int y, bool spawnRight) : Enemy(x, y, spawnRight)
{
	//animations
	idle.PushBack({ 0,0,107,39 });
	//collision
	collider = App->collisions->AddCollider({ position.x, position.y, 107, 39 }, Collider::Type::BOSS, (Module*)App->enemies);
	//--------PATHS------
	//SPAWN PATH TOP
	spawnPathTop.PushBack({ 1.4f,0.0f }, 240, &idle);
	spawnPathTop.PushBack({ 1.12f,0.65f }, 60, &idle);
	spawnPathTop.PushBack({ 1.12f,0.65f }, 240, &idle);
	spawnPathTop.PushBack({ 1.0f,0.0f }, 10, &idle);
	spawnPathTop.loop = false;
	//SPAWN PATH BOT
	spawnPathBot.PushBack({ 1.4f,0.0f }, 240, &idle);
	spawnPathBot.PushBack({ 1.05f,-0.98f }, 120, &idle);
	spawnPathBot.PushBack({ 1.25f,-0.98f }, 120, &idle);
	spawnPathBot.PushBack({ 1.0f,0.0f }, 10, &idle);
	spawnPathBot.loop = false;
	//IDLE PATH WHEN SPAWNING TOP
	idlePathTop.PushBack({ 1.13f,-0.7f }, 180, &idle);//up-right
	idlePathTop.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathTop.PushBack({ 1.01f,0.7f }, 180, &idle);//down-right
	idlePathTop.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathTop.PushBack({ 1.08f,-0.7f }, 180, &idle);//up-right
	idlePathTop.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathTop.PushBack({ 0.82f,0.7f }, 180, &idle);//down-left
	idlePathTop.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathTop.PushBack({ 1.13f,-0.8f }, 275, &idle);//up-right and despawn
	idlePathTop.PushBack({ 2.0f,-0.0f }, 180, &idle);//up-right and despawn
	idlePathTop.loop = false;
	//IDLE PATH WHEN SPAWNING BOT
	idlePathBot.PushBack({ 0.80f,0.8f }, 180, &idle);//down-left
	idlePathBot.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathBot.PushBack({ 0.80f,-0.8f }, 180, &idle);//up-left
	idlePathBot.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathBot.PushBack({ 0.99f,0.8f }, 180, &idle);//up-left
	idlePathBot.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathBot.PushBack({ 0.93f,-0.8f }, 180, &idle);//up-left
	idlePathBot.PushBack({ 1.0f,0.0f }, 10, &idle);
	idlePathBot.PushBack({ 0.87f,0.8f }, 275, &idle);//up-left
	idlePathBot.PushBack({ 0.1f,0.0f }, 10, &idle);//

	idlePathBot.loop = false;
	//other var
	if (spawnPos.y <= 0)
	{
		goingUp = false;
		spawningTop = true;
		despawnLeft = false;

	}
	else {
		goingUp = true;
		spawningTop = false;
		despawnLeft = true;
	}
	health = 70;
	scoreGiven = 400;
	moneyGiven = 1200;
}

void GreenFighterPlane::Update()
{
	if (damaged)
	{
		currentTime++;
		if (currentTime == 5)
		{
			SDL_SetTextureColorMod(App->enemies->gfTexture, 255, 255, 255);
			damaged = false;
			currentTime = 0;
		}


	}
	switch (state)
	{
	case gSPAWNING:
		if (spawningTop) {
			spawnPathTop.Update();
			position = spawnPos + spawnPathTop.GetRelativePosition();
		}
		else {
			spawnPathBot.Update();
			position = spawnPos + spawnPathBot.GetRelativePosition();
		}
		
		if (spawnPathTop.Finished()||spawnPathBot.Finished())
		{
			state = gIDLE;
		}
		if (currentAnim != &idle) {
			currentAnim = &idle;
		}
		
		idlePosition = position;
		break;
	case gIDLE:
		if (spawningTop) {
			idlePathTop.Update();
			position = idlePosition + idlePathTop.GetRelativePosition();
		}
		else {
			idlePathBot.Update();
			position = idlePosition + idlePathBot.GetRelativePosition();
			
		}
		if (currentAnim != &idle) {
			currentAnim = &idle;
		}
		break;
	default:
		break;
	}
	if (goingUp && position.y <= 37) {
		goingUp = false;

	}else if (!goingUp && position.y >=156){
		goingUp = true;
	}
	Enemy::Update();

	shootingFrequency++;
	if (state == gIDLE && shootingFrequency > 60)
	{
		shootingFrequency = 0;
		if (goingUp) {
			App->weapons->SpawnWeapon(WEAPON_TYPE::GF_HOOK, position.x + 53.5f, position.y + 39, 0);

		}
		else {
			App->weapons->SpawnWeapon(WEAPON_TYPE::GF_HOOK, position.x + 53.5f, position.y, 1);

		}
		//SetBulletDirection(this);

		//App->particles->AddParticle(App->particles->enemyBullet, position.x + 32, position.y, Collider::Type::ENEMY_SHOT);
	}
}

void GreenFighterPlane::OnCollision(Collider* collider) {
	damaged = true;
	SDL_SetTextureColorMod(App->enemies->gfTexture, 250, 200, 75);
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

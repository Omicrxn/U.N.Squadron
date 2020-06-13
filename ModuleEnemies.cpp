#include "ModuleEnemies.h"

#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include "MediumCamouflageJet.h"
#include "BigOrangeJet.h"
#include "BigCamouflageJet.h"
#include "SmallCamouflageJet.h"
#include "StealthBomber.h"
#include "BlueJet.h"
#include "Truck.h"
#include "GreenFighterPlane.h"
#include "ModuleParticles.h"

#define SPAWN_MARGIN 50

ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	name = "Enemies";

	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	texture = App->textures->Load("Assets/sprites/enemies/UNSquadronSheet9.gif");
	++activeTextures; ++totalTextures;
	sbTexture = App->textures->Load("Assets/sprites/enemies/UNSquadronSheet10.gif ");
	++activeTextures; ++totalTextures;
	truckTexture = App->textures->Load("Assets/sprites/enemies/UNSquadronSheet24.gif");
	++activeTextures; ++totalTextures;
	enemyDestroyedFx = App->audio->LoadFx("Assets/music/events/enemiesexplotion.wav");
	++activeFx; ++totalFx;

	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// Remove all enemies scheduled for deletion
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->pendingToDelete)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}
update_status ModuleEnemies::Update()
{
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
			enemies[i]->Update();
	}

	HandleEnemiesDespawn();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	activeTextures = activeFx = 0;

	if (!App->textures->Unload(texture)) {
		LOG("Start Screen -> Error unloading the texture.");
		return false;
	}
	--totalTextures;
	if (!App->textures->Unload(sbTexture)) {
		LOG("Start Screen -> Error unloading the texture.");
		return false;
	}
	--totalTextures;
	App->audio->UnloadFx(enemyDestroyedFx);
	--totalFx;

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y, bool spawnRight)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			spawnQueue[i].spawnRight = spawnRight;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].spawnRight == true) {
			if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
			{

				// Spawn a new enemy if the screen has reached a spawn position
				if (spawnQueue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w) + SPAWN_MARGIN)
				{
					LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

					SpawnEnemy(spawnQueue[i]);
					spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
				}
			}
		}
		else {
			if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
			{

				// Spawn a new enemy if the screen has reached a spawn position
				if (spawnQueue[i].x * SCREEN_SIZE < App->render->camera.x  - SPAWN_MARGIN)
				{
					LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

					SpawnEnemy(spawnQueue[i]);
					spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
				}
			}

		}
		
	}
}

void ModuleEnemies::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->despawnLeft == true)
			{
				if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
				{
					LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

					enemies[i]->SetToDelete();
				}
			}
			else
			{
				if (enemies[i]->position.x * SCREEN_SIZE > ((App->render->camera.x) + SPAWN_MARGIN + (SCREEN_WIDTH * SCREEN_SIZE)))
				{
					LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

					enemies[i]->SetToDelete();
				}
			}
		}
	}
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
				case ENEMY_TYPE::MEDIUMCAMOUFLAGEJET:
					enemies[i] = new MediumCamouflageJet(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::BIGORANGEJET:
					enemies[i] = new BigOrangeJet(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::BIGCAMOUFLAGEJET:
					enemies[i] = new BigCamouflageJet(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::BLUEJET:
					enemies[i] = new BlueJet(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::GREENFIGHTERPLANE:
					enemies[i] = new GreenFighterPlane(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::SMALLCAMOUFLAGEJET:
					enemies[i] = new SmallCamouflageJet(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::STEALTHBOMBER:
					enemies[i] = new StealthBomber(info.x, info.y, info.spawnRight);
					break;
				case ENEMY_TYPE::TRUCK:
					enemies[i] = new Truck(info.x, info.y, info.spawnRight);
					break;
			}
			if (info.type == ENEMY_TYPE::STEALTHBOMBER) {
				enemies[i]->texture = sbTexture;
			}
			else {
				enemies[i]->texture = texture;
			}
			enemies[i]->destroyedFx = enemyDestroyedFx;
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1 && c2->type == Collider::Type::PLAYER_SHOT) {
			enemies[i]->OnCollision(c2);
		}
	}
}
#include "ModuleEnemies.h"

#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleAudio.h"

#include "Enemy.h"
#include "MediumCamouflageJet.h"
#include "BigOrangeJet.h"
#include "BigCamouflageJet.h"
#include "BlueJet.h"
#include "GreenFighterPlane.h"
#include "ModuleParticles.h"

#define SPAWN_MARGIN 50

ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	texture = App->textures->Load("Assets/sprites/enemies/UNSquadronSheet9.gif ");
	enemyDestroyedFx = App->audio->LoadFx("Assets/music/explosion.wav");

	return true;
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

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
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
		if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

				SpawnEnemy(spawnQueue[i]);
				spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
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
			if (enemies[i]->right == true)
			{
				if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
				{
					LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

					delete enemies[i];
					enemies[i] = nullptr;
				}
			}
			else
			{
				if (enemies[i]->position.x * SCREEN_SIZE > (App->render->camera.x) + SPAWN_MARGIN + (SCREEN_WIDTH * SCREEN_SIZE))
				{
					LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

					delete enemies[i];
					enemies[i] = nullptr;
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
					enemies[i] = new MediumCamouflageJet(info.x, info.y);
					break;
				case ENEMY_TYPE::BIGORANGEJET:
					enemies[i] = new BigOrangeJet(info.x, info.y);
					break;
				case ENEMY_TYPE::BIGCAMOUFLAGEJET:
					enemies[i] = new BigCamouflageJet(info.x, info.y);
					break;
				case ENEMY_TYPE::BLUEJET:
					enemies[i] = new BlueJet(info.x, info.y);
					break;
				case ENEMY_TYPE::GREENFIGHTERPLANE:
					enemies[i] = new GreenFighterPlane(info.x, info.y);
					break;
			}
			enemies[i]->texture = texture;
			enemies[i]->destroyedFx = enemyDestroyedFx;
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision

			delete enemies[i];
			enemies[i] = nullptr;
			break;
		}
	}
}
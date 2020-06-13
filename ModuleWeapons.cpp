#include "ModuleWeapons.h"

#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleAudio.h"

#include "Weapon.h"
#include "Bomb.h"
#include "S.Shell.h"
#include "Falcon.h"
#include "Ceiling.h"
#include "GF_Hook.h"
#include "SB_Bomb.h"
#include "SB_BombShrapnel.h"
#include "PowerupOrange.h"
#include "PowerupBlue.h"

#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include<cassert>

#define SPAWN_MARGIN 50

ModuleWeapons::ModuleWeapons(bool startEnabled) : Module(startEnabled)
{
	name = "Weapons";

	for (uint i = 0; i < MAX_WEAPONS; ++i)
		weapons[i] = nullptr;
}

ModuleWeapons::~ModuleWeapons()
{

}

bool ModuleWeapons::Start()
{
	texture = App->textures->Load("Assets/sprites/weapons/weapons.png");
	++activeTextures; ++totalTextures;

	weaponDestroyedFx = App->audio->LoadFx("Assets/music/explosionWeapon.wav"); // Change later
	++activeFx; ++totalFx;

	return true;
}

update_status ModuleWeapons::PreUpdate()
{
	// Remove all weapons scheduled for deletion
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr && weapons[i]->pendingToDelete)
		{
			delete weapons[i];
			weapons[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleWeapons::Update()
{
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr)
			weapons[i]->Update();
	}

	DespawnWeapon();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleWeapons::PostUpdate()
{
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr)
			weapons[i]->Draw();
	}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWeapons::CleanUp()
{
	LOG("Freeing all weapons");

	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr)
		{
			delete weapons[i];
			weapons[i] = nullptr;
		}
	}

	activeTextures = activeFx = 0;

	if (!App->textures->Unload(texture)) {
		LOG("Start Screen -> Error unloading the texture.");
		return false;
	}
	--totalTextures;

	App->audio->UnloadFx(weaponDestroyedFx);
	--totalFx;

	return true;
}

void ModuleWeapons::SpawnWeapon(WEAPON_TYPE weaponType, int x, int y)
{
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] == nullptr)
		{
			switch (weaponType)
			{
			case WEAPON_TYPE::BOMB:
				weapons[i] = new Bomb(App->player->GetPlayerPosition().x, App->player->GetPlayerPosition().y);
				break;
			case WEAPON_TYPE::SHELL:
				weapons[i] = new Shell(App->player->GetPlayerPosition().x, App->player->GetPlayerPosition().y);
				break;
			case WEAPON_TYPE::FALCON:
				weapons[i] = new Falcon(App->player->GetPlayerPosition().x, App->player->GetPlayerPosition().y);
				break;
			case WEAPON_TYPE::CEILING:
				weapons[i] = new Ceiling(x, y);
				break;
			case WEAPON_TYPE::POWERUP_ORANGE:
				weapons[i] = new PowerupOrange(orangeSpawn.x, orangeSpawn.y);
				break;
			case WEAPON_TYPE::POWERUP_BLUE:
				weapons[i] = new PowerupBlue(blueSpawn.x, blueSpawn.y);
				break;
			}
			weapons[i]->texture = this->texture;
			weapons[i]->destroyedFx = weaponDestroyedFx;
			break;
		}
	}
}

void ModuleWeapons::SpawnWeapon(WEAPON_TYPE weaponType,int x, int y,unsigned int number){
	assert(number <= 7);
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] == nullptr)
		{
			switch (weaponType)
			{
			case WEAPON_TYPE::GF_HOOK:
				weapons[i] = new GF_Hook(x, y,number);
				break;
			case WEAPON_TYPE::SB_BOMB:
				weapons[i] = new SB_Bomb(x, y,number);
				break;
			case WEAPON_TYPE::SB_SHRAPNEL:
				weapons[i] = new SB_BombShrapnel(x, y, number);
				break;
			}
				
			weapons[i]->texture = this->texture;
			weapons[i]->destroyedFx = weaponDestroyedFx;
			break;
		}
	}
}

void ModuleWeapons::DespawnWeapon()
{
	// Iterate existing weapons
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr)
		{
			// Delete the weapon when it has reached the end of the screen
			if (weapons[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning weapon at %d", weapons[i]->position.x * SCREEN_SIZE);

				weapons[i]->SetToDelete();
			}
			else if (weapons[i]->position.x * SCREEN_SIZE > ((App->render->camera.x) + SPAWN_MARGIN + (SCREEN_WIDTH * SCREEN_SIZE)))
			{
				LOG("DeSpawning weapon at %d", weapons[i]->position.x * SCREEN_SIZE);

				weapons[i]->SetToDelete();
			}
			else if (weapons[i]->position.y * SCREEN_SIZE < (App->render->camera.y) - SPAWN_MARGIN)
			{
				LOG("DeSpawning weapon at %d", weapons[i]->position.x * SCREEN_SIZE);

				weapons[i]->SetToDelete();
			}
			else if (weapons[i]->position.y * SCREEN_SIZE > ((App->render->camera.y) + SPAWN_MARGIN + (SCREEN_HEIGHT * SCREEN_SIZE)))
			{
				LOG("DeSpawning weapon at %d", weapons[i]->position.x * SCREEN_SIZE);

				weapons[i]->SetToDelete();
			}
		}
	}
}

void ModuleWeapons::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr && weapons[i]->GetCollider() == c1)
		{
			weapons[i]->OnCollision(c2); // Notify the weapon of a collision

			delete weapons[i];
			weapons[i] = nullptr;
			break;
		}
	}
}
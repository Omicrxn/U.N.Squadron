#include "ModuleWeapons.h"

#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleAudio.h"

#include "Weapon.h"
#include "Bomb.h"
#include "ModuleParticles.h"

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
	texture = App->textures->Load("Assets/sprites/enemies/UNSquadronSheet9.gif"); // Change later
	++activeTextures; ++totalTextures;

	weaponDestroyedFx = App->audio->LoadFx("Assets/music/explosion.wav"); // Change later
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

bool ModuleWeapons::AddWeapon(WEAPON_TYPE type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (spawnQueue[i].type == WEAPON_TYPE::NO_TYPE)
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

void ModuleWeapons::SpawnWeapon(const WeaponSpawnpoint& info)
{
	// Find an empty slot in the weapons array
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] == nullptr)
		{
			switch (info.type)
			{
			case WEAPON_TYPE::BOMB:
				weapons[i] = new Bomb(info.x, info.y);
				break;
			}
			weapons[i]->texture = texture;
			weapons[i]->destroyedFx = weaponDestroyedFx;
			break;
		}
	}
}

void ModuleWeapons::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_WEAPONS; ++i)
	{
		if (weapons[i] != nullptr && weapons[i]->GetCollider() == c1)
		{
			weapons[i]->OnCollision(c2); //Notify the weapon of a collision

			delete weapons[i];
			weapons[i] = nullptr;
			break;
		}
	}
}
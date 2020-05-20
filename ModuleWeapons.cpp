#include "ModuleWeapons.h"

#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleAudio.h"

#include "Weapon.h"
#include "Bomb.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

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
	texture = App->textures->Load("Assets/sprites/hud/HUD.png"); // Change later
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

void ModuleWeapons::SpawnWeapon(WEAPON_TYPE weaponType)
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
			}
			weapons[i]->texture = this->texture;
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
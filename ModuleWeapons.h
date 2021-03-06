#ifndef __MODULE_WEAPONS_H__
#define __MODULE_WEAPONS_H__

#include "Module.h"

#define MAX_WEAPONS 100

// Create new weapon types:
enum class WEAPON_TYPE
{
	NO_TYPE, BOMB, SHELL, FALCON, CEILING, GF_HOOK, SB_BOMB, SB_SHRAPNEL, POWERUP_BLUE, POWERUP_ORANGE, LEVEL1GUN
};

struct WeaponSpawnpoint
{
	WEAPON_TYPE type = WEAPON_TYPE::NO_TYPE;
	int x, y;
};

class Weapon;
struct SDL_Texture;

class ModuleWeapons : public Module
{
public:
	// Constructor
	ModuleWeapons(bool startEnabled);

	// Destructor
	~ModuleWeapons();

	// Called when the module is activated
	// Loads the necessary textures for the weapons
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all weapons pending to delete
	update_status PreUpdate() override;

	// Called at the middle of the application loop
	// Handles all weapons logic and spawning/despawning
	update_status Update() override;

	// Called at the end of the application loop
	// Iterates all the weapons and draws them
	update_status PostUpdate() override;

	// Called on application exit
	// Destroys all active weapons left in the array
	bool CleanUp() override;

	// Called when an weapon collider hits another collider
	// The weapon is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2) override;

	// Spawns a new weapon using the data from the queue
	void SpawnWeapon(WEAPON_TYPE weaponType,int x=0, int y=0);

	// Destroys any weapons that have moved outside the camera limits
	void DespawnWeapon();
	void SpawnWeapon(WEAPON_TYPE weaponType,int x, int y, unsigned int number);

private:
	// All spawned weapons in the scene
	Weapon* weapons[MAX_WEAPONS] = { nullptr };

	// The weapons sprite sheet
	SDL_Texture* texture = nullptr;

	// The audio fx for destroying an weapon
	int weaponDestroyedFx = 0;
};

#endif // __MODULE_WEAPONS_H__
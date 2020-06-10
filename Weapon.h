#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Weapon
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Weapon(int x, int y);

	// Destructor
	virtual ~Weapon();

	// Returns the weapon's collider
	const Collider* GetCollider() const;

	// Called from inhering weapon's Udpate
	// Updates animation and collider position
	virtual void Update();

	// Called from ModuleWeapon's Update
	virtual void Draw();

	// Collision response
	// Triggers an animation and a sound fx
	virtual void OnCollision(Collider* collider);
	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

public:
	// The current position in the world
	iPoint position;

	// The weapon's texture
	SDL_Texture* texture = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the weapon removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// The weapon's collider
	Collider* collider = nullptr;

	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;
};

#endif // __WEAPON_H__
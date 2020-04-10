#pragma once
#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__


#include "Application.h"
#include "Animation.h"
#include "p2Point.h"
#include "Module.h"
#include"SDL.h"
#include "ModuleCollisions.h"

#define MAX_ACTIVE_PARTICLES 200
struct SDL_Texture;

struct Collider;

struct Particle
{
public:
	// Constructor
	Particle();
	

	// Copy constructor
	Particle(const Particle& p);

	~Particle();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();

public:
	// Defines the position in the screen
	iPoint position;

	// Defines the speed at which the particle will move (pixels per second)
	iPoint speed;

	// A set of rectangle sprites
	Animation anim;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the time when the particle will be spawned
	int frameCount = 0;

	// Defines the total amount of time during which the particle will be active (in miliseconds)
	Uint32 lifetime = 0;
	// The particle's collider
	Collider* collider = nullptr;
};
class ModuleParticles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles();

	//Destructor
	~ModuleParticles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start() override;

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	update_status Update() override;

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	update_status PostUpdate() override;

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp() override;

	// Called when a particle collider hits another collider
	void OnCollision(Collider* c1, Collider* c2) override;


	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	void AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType = Collider::Type::NONE, uint delay = 0);

private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

	// An index to the last added particle
	uint lastParticle = 0;

public:
	//Template particle for an explosion
	Particle explosion;
	Particle bullet;
	Particle enemyBullet;
};
#endif
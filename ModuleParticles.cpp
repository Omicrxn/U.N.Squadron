#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleRenderer.h"
#include "ModuleCollisions.h"

#include "SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	name = "particles";

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	particlesTexture = App->textures->Load("Assets/sprites/explosions/explosions.png");
	++totalTextures;

	LOG("Loading weapons particles");
	weaponsParticlesTexture = App->textures->Load("Assets/sprites/weapons/weapons.png");
	++totalTextures;

	// Bullets Animaton 
	bullet1.anim.PushBack({ 456, 8, 13, 3 });
	bullet2.anim.PushBack({ 470, 6, 12, 6 });
	bullet3.anim.PushBack({ 483, 3, 13, 11 });
	bullet4.anim.PushBack({ 498, 0, 13, 17 });

	bullet1.anim.loop = bullet2.anim.loop = bullet3.anim.loop = bullet4.anim.loop = true;
	bullet1.speed.x = bullet2.speed.x = bullet3.speed.x = bullet4.speed.x = 6.0f;
	bullet1.lifetime = bullet2.lifetime = bullet3.lifetime = bullet4.lifetime = 180;

	// Enemy Bullet Animaton
	enemyBullet.anim.PushBack({ 44, 370, 5 , 7 });
	enemyBullet.anim.PushBack({ 78, 372, 5, 5 });
	enemyBullet.anim.PushBack({ 110, 371, 15, 6 });
	enemyBullet.anim.loop = true;
	enemyBullet.anim.speed = 0.075f;
	enemyBullet.lifetime = 180;
	

	// Explosion Animation
	explosion.anim.PushBack({ 27, 256, 34, 39 });
	explosion.anim.PushBack({ 98, 261, 44, 36 });
	explosion.anim.PushBack({ 164, 263, 33, 27 });
	explosion.anim.PushBack({ 223, 262, 31, 29 });
	explosion.anim.loop = true;
	explosion.anim.speed = 0.075f;
	
	// Explosion of the enemies
	enemyExplosion.anim.PushBack({ 27, 313, 24, 25 });
	enemyExplosion.anim.PushBack({ 97, 310, 31, 29 });
	enemyExplosion.anim.PushBack({ 159, 310, 31, 28 });
	enemyExplosion.anim.PushBack({ 217, 315, 35, 27 });
	enemyExplosion.anim.speed = 0.075f;

	// Stealth bomber firecannon
	sbFirecannon.anim.PushBack({ 25,454,16,12 });
	sbFirecannon.anim.loop = false;
	sbFirecannon.anim.speed = 0;
	sbFirecannon.speed.x = -2;

	// Falcon explosion animation
	falconExplosion.anim.PushBack({ 154, 0, 23, 26 });
	falconExplosion.anim.PushBack({ 178, 0, 24, 26 });
	falconExplosion.anim.PushBack({ 203, 0, 26, 28 });
	falconExplosion.anim.PushBack({ 230, 0, 26, 28 });
	falconExplosion.anim.PushBack({ 284, 0, 26, 28 });
	falconExplosion.anim.speed = 0.075f;
	bombExplosion.anim.loop = false;
	falconExplosion.isWeapon = true;

	// Bomb explosions animation
	bombExplosion.anim.PushBack({ 28, 57, 28, 33 });
	bombExplosion.anim.PushBack({ 57, 57, 24, 33 });
	bombExplosion.anim.PushBack({ 82, 57, 28, 33 });
	bombExplosion.anim.PushBack({ 111, 57, 26, 33 });
	bombExplosion.anim.PushBack({ 138, 57, 28, 33 });
	bombExplosion.anim.PushBack({ 167, 57, 28, 33 });
	bombExplosion.anim.speed = 0.075f;
	bombExplosion.anim.loop = false;
	bombExplosion.isWeapon = true;

	// Ceiling explosions animation
	ceilingExplosion.anim.PushBack({ 210, 151, 8, 5 });
	ceilingExplosion.anim.PushBack({ 190, 148, 19, 8 });
	ceilingExplosion.anim.PushBack({ 0, 149, 16, 7 });
	ceilingExplosion.anim.PushBack({ 222, 142, 14, 14 });
	ceilingExplosion.anim.speed = 0.075f;
	ceilingExplosion.speed.x = 1;
	ceilingExplosion.anim.loop = false;
	ceilingExplosion.isWeapon = true;

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
			--particlesCount;
		}
	}

	App->textures->Unload(particlesTexture);
	--totalTextures;
	App->textures->Unload(weaponsParticlesTexture);
	--totalTextures;

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			delete particles[i];
			particles[i] = nullptr;
			--particlesCount;
			break;
		}
	}
}

update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			delete particle;
			particles[i] = nullptr;
			--particlesCount;
		}
	}
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			if (particle->isWeapon) {
				App->render->Blit(weaponsParticlesTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));

			}
			else {
				App->render->Blit(particlesTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));

			}
		}
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			Particle* p = new Particle(particle);

			p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			p->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE) {
				p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);
			}

			particles[i] = p;
			++particlesCount;
			break;
		}
	}
}

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime),isWeapon(p.isWeapon)
{

}
Particle::~Particle()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}
bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the spawnTime is reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.Finished())
			ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

		if (collider != nullptr)
			collider->SetPos(position.x, position.y);
	}

	return ret;
}
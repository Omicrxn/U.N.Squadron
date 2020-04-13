#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleRenderer.h"
#include "ModuleCollisions.h"

#include "SDL_timer.h"

ModuleParticles::ModuleParticles()
{
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

	//Bullet Animaton
	bullet.anim.PushBack({ 27,218,17,6 });
	bullet.anim.PushBack({ 64,218,12,6 });
	bullet.anim.PushBack({ 91,217,17,7 });
	bullet.anim.PushBack({ 128,217,12,7 });
	bullet.anim.PushBack({ 155,217,17,7 });
	bullet.anim.PushBack({ 192,217,12,7 });
	bullet.anim.loop = true;
	bullet.anim.speed = 0.05f;
	bullet.speed.x = 5.0f;
	bullet.lifetime = 180;

	//Enemy Bullet Animaton
	enemyBullet.anim.PushBack({ 27,218,17,6 });
	enemyBullet.anim.PushBack({ 64,218,12,6 });
	enemyBullet.anim.PushBack({ 91,217,17,7 });
	enemyBullet.anim.PushBack({ 128,217,12,7 });
	enemyBullet.anim.PushBack({ 155,217,17,7 });
	enemyBullet.anim.PushBack({ 192,217,12,7 });
	enemyBullet.anim.loop = true;
	enemyBullet.anim.speed = 0.05f;
	enemyBullet.speed.x = -5.0f;
	enemyBullet.lifetime = 180;

	//Explosion Animation
	explosion.anim.PushBack({ 27, 128, 40, 41 });
	explosion.anim.PushBack({ 78, 127, 41, 64 });
	explosion.anim.PushBack({ 130, 127, 45, 57 });
	explosion.anim.PushBack({ 188, 127, 42, 43 });
	explosion.anim.PushBack({ 244, 127, 37, 19 });
	explosion.anim.loop = true;
	explosion.anim.speed = 0.1f;

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
		}
	}
	App->textures->Unload(particlesTexture);


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
			App->render->Blit(particlesTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
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
frameCount(p.frameCount), lifetime(p.lifetime)
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
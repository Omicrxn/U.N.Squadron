#include "Bomb.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Bomb::Bomb(int x, int y) : Weapon(x, y)
{
	drop1.PushBack({ 0, 80, 13, 10 });
	drop2.PushBack({ 14, 76, 13, 14 });

	path.PushBack({ 1.5f, 2.0f }, 15, &drop1);
	path.PushBack({ 1.5f, 2.0f }, 85, &drop2);

	collider = App->collisions->AddCollider({ position.x, position.y, 30, 9 }, Collider::Type::WEAPON, (Module*)App->weapons);
}

void Bomb::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Weapon::Update();
}

void Bomb::OnCollision(Collider* collider)
{
	App->particles->AddParticle(App->particles->bombExplosion, position.x, position.y);
	App->audio->PlayFx(destroyedFx, 0);
}
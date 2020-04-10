#include "MediumCamouflageJet.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRenderer.h"

MediumCamouflageJet::MediumCamouflageJet(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 225,151,29,15 });
	currentAnim = &fly;

	// Have the medium camouflage jet describe a path in the screen

	collider = App->collisions->AddCollider({ 0, 0, 32, 16 }, Collider::Type::ENEMY, (Module*)App->enemies);
}

void MediumCamouflageJet::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
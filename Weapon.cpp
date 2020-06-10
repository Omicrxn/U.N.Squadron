#include "Weapon.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRenderer.h"
#include "ModuleHUD.h"
#include "ModulePlayer.h"

Weapon::Weapon(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Weapon::~Weapon()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Weapon::GetCollider() const
{
	return collider;
}

void Weapon::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Weapon::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Weapon::OnCollision(Collider* collider)
{
	if (App->player->currentWeapon == App->player->BOMB)
	{
		App->particles->AddParticle(App->particles->bombExplosion, position.x, position.y);
	}
	else
	{
		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y);
	}
	App->audio->PlayFx(3, 0);
}

void Weapon::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}
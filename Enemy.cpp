#include "Enemy.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRenderer.h"
#include "ModulePlayer.h"
#include "ModuleDebugInfo.h"
#include "ModuleWeapons.h"

Enemy::Enemy(int x, int y,bool spawnRight) : position(x, y)
{
	spawnPos = position;
	despawnLeft = spawnRight;
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Enemy::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* collider)
{
	if (health > 1) {
		health--;
	}
	else {
		App->particles->AddParticle(App->particles->enemyExplosion, position.x, position.y);
		App->audio->PlayFx(destroyedFx, 0);

		App->player->score += scoreGiven;

		if (!App->debugInfo->maxMoney) {
			App->player->money += moneyGiven;
		}

		if (powerupSpawn == 1) {
			App->weapons->orangeSpawn.x = this->position.x;
			App->weapons->orangeSpawn.y = this->position.y;
		}
		else if (powerupSpawn == 2) {
			App->weapons->blueSpawn.x = this->position.x;
			App->weapons->blueSpawn.y = this->position.y;
		}

		this->SetToDelete();
	}
}

void Enemy::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}
#include "SetBulletDirection.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"

#include <math.h>

int speed = 3;
float angle;

void SetBulletDirection(Enemy* enemy)
{
	if (App->player->GetPlayerPosition().y > enemy->position.y)
	{
		if (App->player->GetPlayerPosition().x < enemy->position.x)
		{
			angle = atan(((float)enemy->position.x - (float)App->player->GetPlayerPosition().x)
					   / ((float)App->player->GetPlayerPosition().y - (float)enemy->position.y));
			App->particles->enemyBullet.speed.x = sin(angle) * (-speed);
		}
		else if (App->player->GetPlayerPosition().x > enemy->position.x)
		{
			angle = atan(((float)App->player->GetPlayerPosition().x - (float)enemy->position.x)
					   / ((float)App->player->GetPlayerPosition().y - (float)enemy->position.y));
			App->particles->enemyBullet.speed.x = sin(angle) * speed;
		}

		App->particles->enemyBullet.speed.y = cos(angle) * speed;
	}
	else if (App->player->GetPlayerPosition().y < enemy->position.y)
	{
		if (App->player->GetPlayerPosition().x < enemy->position.x)
		{
			angle = atan(((float)enemy->position.x - (float)App->player->GetPlayerPosition().x)
					   / ((float)enemy->position.y - (float)App->player->GetPlayerPosition().y));
			App->particles->enemyBullet.speed.x = sin(angle) * (-speed);
		}
		else if (App->player->GetPlayerPosition().x > enemy->position.x)
		{
			angle = atan(((float)App->player->GetPlayerPosition().x - (float)enemy->position.x)
				       / ((float)enemy->position.y - (float)App->player->GetPlayerPosition().y));
			App->particles->enemyBullet.speed.x = sin(angle) * speed;
		}
		
		App->particles->enemyBullet.speed.y = cos(angle) * (-speed);
	}
}
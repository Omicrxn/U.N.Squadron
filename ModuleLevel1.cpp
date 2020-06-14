#include "ModuleLevel1.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "ModuleHUD.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"
#include "ModuleWeapons.h"
#include "Truck.h"
#include "Turret.h"

ModuleLevel1::ModuleLevel1(bool startEnabled) : Module(startEnabled) {
	name = "Level 1";
}

ModuleLevel1::~ModuleLevel1() {}

bool ModuleLevel1::Start() {
	bool ret = true;

	numSpawnedEnemies = 0;

	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/BonusLevel.png");
	++activeTextures; ++totalTextures;

	// Music
	App->audio->PlayMusic("Assets/music/soundtrack/cave.ogg");
	
	skyHalf1 = { 0, 0, 6602, 288 };
	skyHalf2 = { 6603, 0, 6602, 288 };
	mountainsHalf1 = { 0, 291, 6602, 224 };
	mountainsHalf2 = { 6603, 291, 6602, 224 };
	floorHalf1 = { 0, 518, 6602, 48 };
	floorHalf2 = { 6603, 518, 6602, 48 };

	App->enemies->AddEnemy(ENEMY_TYPE::TRUCK, 1600, 440);
	App->enemies->AddEnemy(ENEMY_TYPE::TRUCK, 1700, 440);
	App->enemies->AddEnemy(ENEMY_TYPE::TRUCK, 1800, 440);

	App->enemies->AddEnemy(ENEMY_TYPE::TURRET, 50, 50);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	currentPosition = LEFTUP;
	currentDirection = RIGHTDIR;

	App->particles->Enable();
	App->player->Enable();
	App->enemies->Enable();
	App->HUD->Enable();
	App->collisions->Enable();
	App->weapons->Enable();

	return ret;
}

update_status ModuleLevel1::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	// Setting the current position and the current direction
	if (currentDirection == RIGHTDIR)
	{
		if (App->render->camera.x <= 6602) {
			if (App->render->camera.y <= 791) {
				currentPosition = LEFTUP;
			}
			else if (App->render->camera.y > 791) {
				currentPosition = LEFTDOWN;
			}
		}
		else if (App->render->camera.x > 6602) {
			if (App->render->camera.y >= 791) {
				if (App->render->camera.x <= 5000 + SCREEN_WIDTH) {
					currentPosition = RIGHTDOWN;
				}
				else if (App->render->camera.x > 5000 + SCREEN_WIDTH) {
					currentPosition = RIGHTUP;
				}
			}
			else if (App->render->camera.y < 791) {
				currentPosition = RIGHTUP;
				if (App->render->camera.y < -701 && currentDirection == RIGHTDIR) {
					currentDirection = LEFTDIR;
				}
			}
		}
	}
	else if (currentDirection == LEFTDIR)
	{
		if (App->render->camera.x <= 6602) {
			if (App->render->camera.y <= 791) {
				currentPosition = LEFTUP;
				if (App->render->camera.x < 0) {
					currentDirection = RIGHTDIR;
				}
			}
			else if (App->render->camera.y > 791) {
				if (App->render->camera.x >= 5000) {
					currentPosition = LEFTDOWN;
				}
				else if (App->render->camera.x < 5000) {
					currentPosition = LEFTUP;
				}
			}
		}
		else if (App->render->camera.x > 6602) {
			if (App->render->camera.y >= 791) {
				currentPosition = RIGHTDOWN;
			}
			else if (App->render->camera.y < 791) {
				currentPosition = RIGHTUP;
			}
		}
	}

	//Handling the movement of the camera and the player at each moment
	if (currentDirection == RIGHTDIR) {
		if (currentPosition == LEFTUP) {
			App->render->camera.x += cameraSpeed * SCREEN_SIZE;
			App->render->camera.y += (cameraSpeed / proportion) * SCREEN_SIZE;

			App->player->position.x += cameraSpeed;
			App->player->position.y += cameraSpeed / proportion;
		}
		else if (currentPosition == LEFTDOWN) {
			App->render->camera.x += cameraSpeed * SCREEN_SIZE;

			App->player->position.x += cameraSpeed;
		}
		else if (currentPosition == RIGHTDOWN) {
			App->render->camera.x += cameraSpeed * SCREEN_SIZE;

			App->player->position.x += cameraSpeed;
		}
		else if (currentPosition == RIGHTUP) {
			App->render->camera.x += cameraSpeed * SCREEN_SIZE;
			App->render->camera.y -= (cameraSpeed / proportion) * SCREEN_SIZE;

			App->player->position.x += cameraSpeed;
			App->player->position.y -= cameraSpeed / proportion;
		}
	}
	else if (currentDirection == LEFTDIR) {
		if (currentPosition == RIGHTUP) {
			App->render->camera.x -= cameraSpeed * SCREEN_SIZE;
			App->render->camera.y += (cameraSpeed / proportion) * SCREEN_SIZE;

			App->player->position.x -= cameraSpeed;
			App->player->position.y += cameraSpeed / proportion;
		}
		else if (currentPosition == RIGHTDOWN) {
			App->render->camera.x -= cameraSpeed * SCREEN_SIZE;

			App->player->position.x -= cameraSpeed;
		}
		else if (currentPosition == LEFTDOWN) {
			App->render->camera.x -= cameraSpeed * SCREEN_SIZE;

			App->player->position.x -= cameraSpeed;
		}
		else if (currentPosition == LEFTUP) {
			App->render->camera.x -= cameraSpeed * SCREEN_SIZE;
			App->render->camera.y -= (cameraSpeed / proportion) * SCREEN_SIZE;

			App->player->position.x -= cameraSpeed;
			App->player->position.y -= cameraSpeed / proportion;
		}
	}
	
	return ret;
}

update_status ModuleLevel1::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	/*Render Sky*/
	if (!App->render->Blit(backgroundTexture, 0, -70, &skyHalf1, 0.3)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, 6603, -70, &skyHalf2, 0.3)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	/*Render Mountains*/
	if (!App->render->Blit(backgroundTexture, 0, 84, &mountainsHalf1, 0.5)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, 6603, 84, &mountainsHalf2, 0.5)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	/*Render Floor*/
	if (!App->render->Blit(backgroundTexture, 0, 308, &floorHalf1, 0.5)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, 6603, 308, &floorHalf2, 0.5)) {
		LOG("Cannot blit the texture in ModuleLevel1 %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	
	return ret;
}

bool ModuleLevel1::CleanUp() {
	bool ret = true;

	activeTextures = 0;

	if (!App->textures->Unload(backgroundTexture)) {
		LOG("Error unloading background texture in lvl 1");
		ret = false;
	}
	--totalTextures;

	App->render->camera.x = App->render->camera.y = 0;
	App->audio->StopMusic();

	App->player->Disable();
	App->enemies->Disable();
	App->particles->Disable();
	App->HUD->Disable();
	App->collisions->Disable();
	App->fonts->Disable();
	App->weapons->Disable();
	
	return ret;
}
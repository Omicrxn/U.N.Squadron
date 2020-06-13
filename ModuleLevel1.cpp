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
	
	sky = { 0, 0, 3301, 168 };
	mountains = { 0, 227, 3301, 224 };
	floor = { 0, 454, 3301, 48 };

	// Position
	skyPos = { 0, 0 };
	skyPos2 = { (float)sky.w, 0 };
	skySpeed = 0.5;

	mountainsPos = { 0, 85 };
	mountainsPos2 = { (float)mountains.w, 85 };
	mountainsSpeed = 0.5;

	floorPos = { 0, 308 };
	floorPos2 = { (float)floor.w, 308 };
	floorSpeed = 0.5;

	App->enemies->AddEnemy(ENEMY_TYPE::TRUCK, 1500, 770);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

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

	if (App->render->camera.y <= 780) {
		App->player->position.x++;
		App->player->position.y++;
		App->render->camera.x += SCREEN_SIZE;
		App->render->camera.y += SCREEN_SIZE;
	}
	else if (App->render->camera.y > 780) {
		if (App->render->camera.x <= 2000) {
			App->render->camera.x += SCREEN_SIZE;
			App->player->position.x++;
		}
		if (App->render->camera.x > 2000) {
			App->player->position.y--;
			App->player->position.x++;

			App->render->camera.x += SCREEN_SIZE;
			App->render->camera.y -= SCREEN_SIZE;
		}
	}
	
	return ret;
}

update_status ModuleLevel1::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	/*Render Sky*/
	if (!App->render->Blit(backgroundTexture, (int)skyPos.x, (int)skyPos.y, &sky, skySpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	/*Render Mountains*/
	if (!App->render->Blit(backgroundTexture, (int)mountainsPos.x, (int)mountainsPos.y, &mountains, mountainsSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	/*Render Floor*/
	if (!App->render->Blit(backgroundTexture, (int)floorPos.x, (int)floorPos.y, &floor, floorSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
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
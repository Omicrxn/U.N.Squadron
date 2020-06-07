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

ModuleLevel1::ModuleLevel1(bool startEnabled) : Module(startEnabled) {
	name = "Level 1";
}

ModuleLevel1::~ModuleLevel1() {}

bool ModuleLevel1::Start() {
	bool ret = true;

	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/Level1.png");
	++activeTextures; ++totalTextures;

	// Music
	App->audio->PlayMusic("Assets/music/soundtrack/front-line-base.ogg");

	firsttopLayer = { 4, 5, 256, 149 };
	secondtopLayer = { 4, 5, 256, 149 };

	firstBottomLayer = { 4, 157, 256, 149 };
	secondBottomLayer = { 260, 157, 256, 149 };

	//Background and position in screen
	// top
	firsttopPos = { 0, 0 };
	firsttopPos2 = { (float)firsttopLayer.w, 0 };
	topSpeed = 0.5;

	secondTopPos = { 256, 0 };
	secondTopPos2 = { (float)secondtopLayer.w, 0 };
	topSpeed = 0.5;

	// bot
	firstBotPos = { 0, 145 };
	firstBotPos2 = { (float)firstBottomLayer.w, 145};
	firstSpeed = 0.5;

	secondBotPos = { 256, 145 };
	secondBotPos2 = { (float)secondBottomLayer.w, 145 };
	secondSpeed = 0.5;

	return ret;
}

update_status ModuleLevel1::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	App->render->camera.x += SCREEN_SIZE;

	//First scrolling
	InfiniteScrolling(&firsttopPos, &firsttopPos2, &firstBotPos, &firstBotPos2, &firsttopLayer, &firstBottomLayer, firstSpeed);

	InfiniteScrolling(&firsttopPos, &firsttopPos2, &firstBotPos, &firstBotPos2, &firsttopLayer, &firstBottomLayer, secondSpeed);

	//Second scrolling
	InfiniteScrolling(&secondTopPos, &secondTopPos2, &secondBotPos, &secondBotPos2, &secondtopLayer, &secondBottomLayer, firstSpeed);

	InfiniteScrolling(&secondTopPos, &secondTopPos2, &secondBotPos, &secondBotPos2, &secondtopLayer, &secondBottomLayer, secondSpeed);

	return ret;
}

update_status ModuleLevel1::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Render first layers
	if (!App->render->Blit(backgroundTexture, (int)firsttopPos.x, (int)firsttopPos.y, &firsttopLayer, topSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstBotPos.x, (int)firstBotPos.y, &firstBottomLayer, firstSpeed)) {
		LOG("Cannot blit the texture in ModulePlaer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	// Render second layers
	if (!App->render->Blit(backgroundTexture, (int)secondTopPos.x, (int)secondTopPos.y, &secondtopLayer, topSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondBotPos.x, (int)secondBotPos.y, &secondBottomLayer, secondSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}

	return ret;
}

void ModuleLevel1::InfiniteScrolling(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2, SDL_Rect* topLayer, SDL_Rect* botLayer, float speed) {
	if (bot != nullptr) {
		if ((App->render->camera.x * speed) >= top2->x * SCREEN_SIZE && top->x * SCREEN_SIZE <= top2->x * SCREEN_SIZE) {
			top->x = top2->x + topLayer->w;
			bot->x = bot2->x + botLayer->w;
		}
		else if ((App->render->camera.x * speed) >= top->x * SCREEN_SIZE && top2->x * SCREEN_SIZE <= top->x * SCREEN_SIZE) {
			top2->x = top->x + topLayer->w;
			bot2->x = bot->x + botLayer->w;
		}
	}
	else {

		if ((App->render->camera.x * speed) >= top2->x * SCREEN_SIZE && top->x * SCREEN_SIZE <= top2->x * SCREEN_SIZE) {
			top->x = top2->x + topLayer->w;
		}
		else if ((App->render->camera.x * speed) >= top->x * SCREEN_SIZE && top2->x * SCREEN_SIZE <= top->x * SCREEN_SIZE) {
			top2->x = top->x + topLayer->w;
		}
	}

	top = nullptr;
	top2 = nullptr;
	bot = nullptr;
	bot2 = nullptr;
	topLayer = nullptr;
	botLayer = nullptr;
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
	
	return ret;
}
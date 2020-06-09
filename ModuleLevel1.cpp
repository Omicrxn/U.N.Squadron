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
	sky = { 11,8,256,111 };
	mountains ={ 11,132,256,32 };
	//Background and position in screen
	// top
	mountainsPos = { 0, (float)sky.h };
	mountainsTwoPos = { (float)mountains.w, (float)sky.h };
	mountainsSpeed = 0.15;

	//secondTopPos = { 256, 0 };
	//secondTopPos2 = { (float)secondtopLayer.w, 0 };
	//topSpeed = 0.5;

	//// bot
	//firstBotPos = { 0, 145 };
	//firstBotPos2 = { (float)firstBottomLayer.w, 145};
	//firstSpeed = 0.5;

	//secondBotPos = { 256, 145 };
	//secondBotPos2 = { (float)secondBottomLayer.w, 145 };
	//secondSpeed = 0.5;

	return ret;
}

update_status ModuleLevel1::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	App->render->camera.x += SCREEN_SIZE;

	//First scrolling
	InfiniteScrolling(&mountainsPos, &mountainsTwoPos, &mountains, mountainsSpeed);
	return ret;
}

update_status ModuleLevel1::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	/*Render Sky*/
	if (!App->render->Blit(backgroundTexture, 0, 0, &sky, 0)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	/*Render Mountains*/
	if (!App->render->Blit(backgroundTexture, mountainsPos.x, mountainsPos.y, &mountains, mountainsSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, mountainsTwoPos.x, mountainsTwoPos.y, &mountains, mountainsSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	return ret;
}

void ModuleLevel1::InfiniteScrolling(fPoint* top, fPoint* top2, SDL_Rect* topLayer, float speed) {

	if ((App->render->camera.x * speed) >= top2->x * SCREEN_SIZE && top->x * SCREEN_SIZE <= top2->x * SCREEN_SIZE) {
		top->x = top2->x + topLayer->w;
		
	}
	else if ((App->render->camera.x * speed) >= top->x * SCREEN_SIZE && top2->x * SCREEN_SIZE <= top->x * SCREEN_SIZE) {
		top2->x = top->x + topLayer->w;
		
	}
	
	top = nullptr;
	top2 = nullptr;
	topLayer = nullptr;
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
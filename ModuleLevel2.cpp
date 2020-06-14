#include "ModuleLevel2.h"
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

ModuleLevel2::ModuleLevel2(bool startEnabled) : Module(startEnabled) {
	name = "Level 2";
}

ModuleLevel2::~ModuleLevel2() {}

bool ModuleLevel2::Start() {
	bool ret = true;

	numSpawnedEnemies = 0;

	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/ThunderStorm_SpriteSheet.png");
	++activeTextures; ++totalTextures;

	// Music
	App->audio->PlayMusic("Assets/music/soundtrack/airDivision.ogg");
	
	// Center rect
	centerLayer = { 0,100,256,62 };
	
	// firstTop & firstBottom
	firstTopLayer = { 0,81,256,16 };
	firstBottomLayer = { 0,165,256,17 };

	// secondTop & secondBottom
	secondTopLayer = { 0,52,256,27 };
	secondBottomLayer = { 0,184,256,30 };

	// thirdTop & thirdTop
	thirdTopLayer = { 0,0,256,45 };
	thirdBottomLayer = { 0,217,256,33 };

	// Background position in screen
	// center
	centerPos = { 0,85 };
	centerPos2 = { (float)centerLayer.w,85 };
	centerSpeed = 0.5;
	
	// first
	firstTopPos = { 0,69 };
	firstBotPos = { 0,147 };
	firstTopPos2 = { (float)firstTopLayer.w,69 };
	firstBotPos2 = { (float)firstBottomLayer.w,147 };
	firstSpeed = 1.0;
	
	// second
	secondTopPos = { 0,42 };
	secondBotPos = { 0,164 };
	secondTopPos2 = { (float)secondTopLayer.w,42 };
	secondBotPos2 = { (float)secondBottomLayer.w,164 };
	secondSpeed = 1.5;
	
	// third
	thirdTopPos = { 0,0 };
	thirdBotPos = { 0,(float)(SCREEN_HEIGHT - thirdBottomLayer.h) };
	thirdTopPos2 = { (float)thirdTopLayer.w,0 };
	thirdBotPos2 = { (float)thirdBottomLayer.w,(float)(SCREEN_HEIGHT-thirdBottomLayer.h) };
	thirdSpeed = 2.0;

	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 400, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 425, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 450, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 475, 50);

	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 500, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 525, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 550, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 575, SCREEN_HEIGHT - 70);

	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 600, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 625, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 650, SCREEN_HEIGHT - 70, false);

	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 700, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 725, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 750, 50, false);

	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 900, 50, false );
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 925, 50, false );
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 950, 50, false);

	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 900, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 925, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 950, SCREEN_HEIGHT - 70, false);

	// 3 orange jet arriba y 3 big camouflage abajo a la vez (U) izquierda
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 1100, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 1125, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 1150, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 1100, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 1125, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 1150, 50, false);

	// 4 small (2 arriba 2 abajo) derecha
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 1700, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 1725, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 1700, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 1725, SCREEN_HEIGHT - 70);
	//HERE
	// 4 bluejet (2 arriba 2 abajo) izquierda
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 1800, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 1825, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 1800, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 1825, SCREEN_HEIGHT - 70, false);

	// 4 small (2 arriba 2 abajo) derecha
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 2400, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 2425, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 2400, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 2425, SCREEN_HEIGHT - 70);

	// 1 bluejet arriba y 1 abajo izquierda
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 2500, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 2500, SCREEN_HEIGHT - 70, false);

	// 4 small (2 arriba 2 abajo) derecha
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3100, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3125, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3100, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3125, SCREEN_HEIGHT - 70);

	// 3 bluejets (arriba, enmedio y abajo) derecha
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 3435, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 3400, SCREEN_HEIGHT/2);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 3435, SCREEN_HEIGHT - 70);

	// 4 small (2 arriba 2 abajo) derecha
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3700, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3725, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3700, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 3725, SCREEN_HEIGHT - 70);

	// 4 medium camouflage jet abajo a la derecha
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4000, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4025, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4050, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4075, SCREEN_HEIGHT - 70);

	// 6 big camouflage jet (3 arriba y 3 abajo izquierda)
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4200, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4225, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4250, 50, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4200, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4225, SCREEN_HEIGHT - 70, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 4250, SCREEN_HEIGHT - 70, false);

	// 4 medium camouflage en linea desde el medio derecha
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4800, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4825, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4850, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 4875, SCREEN_HEIGHT / 2);

	// 4 medium desde arriba derecha
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5100, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5125, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5150, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5175, 50);

	// 4 medium desde arriba derecha
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5400, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5425, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5450, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 5475, 50);

	// 4 big orange desde arriba a la derecha
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 5700, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 5725, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 5750, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 5775, 50);

	// 4 big camouflage arriba a la derecha
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 6000, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 6025, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 6050, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGCAMOUFLAGEJET, 6075, 50);

	// 3 blue jet medio derecha
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 6300, SCREEN_HEIGHT/2);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 6325, SCREEN_HEIGHT/2);
	App->enemies->AddEnemy(ENEMY_TYPE::BLUEJET, 6350, SCREEN_HEIGHT/2);

	// 4 small medio derecha
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6600, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6625, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6650, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6675, SCREEN_HEIGHT / 2);

	// 4 small medio
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6900, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6925, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6950, SCREEN_HEIGHT / 2);
	App->enemies->AddEnemy(ENEMY_TYPE::SMALLCAMOUFLAGEJET, 6975, SCREEN_HEIGHT / 2);

	// 4 medium desde arriba derecha
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 7200, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 7225, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 7250, 50);
	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 7275, 50);

	// 4 orange desde abajo a la derecha
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 7500, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 7525, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 7550, SCREEN_HEIGHT - 70);
	App->enemies->AddEnemy(ENEMY_TYPE::BIGORANGEJET, 7575, SCREEN_HEIGHT - 70);

 //   //SUB BOSS GREEN FIGHTER PLANE
	App->enemies->AddEnemy(ENEMY_TYPE::GREENFIGHTERPLANE, SCREEN_WIDTH/2, 0, true);

	App->enemies->AddEnemy(ENEMY_TYPE::GREENFIGHTERPLANE, SCREEN_WIDTH/2-5, 256, true);

	//BOSS: STEALTH BOMBER
	/*App->enemies->AddEnemy(ENEMY_TYPE::STEALTHBOMBER, -139, SCREEN_HEIGHT, false);*/

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

update_status ModuleLevel2::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;
	
	App->render->camera.x += SCREEN_SIZE;

	// Center layers movement
	InfiniteScrolling(&centerPos, &centerPos2, nullptr, nullptr, &centerLayer, nullptr, centerSpeed);

	// First layers movement
	InfiniteScrolling(&firstTopPos, &firstTopPos2, &firstBotPos, &firstBotPos2, &firstTopLayer, &firstBottomLayer, firstSpeed);
	
	// Second layer movement
	InfiniteScrolling(&secondTopPos, &secondTopPos2, &secondBotPos, &secondBotPos2, &secondTopLayer, &secondBottomLayer, secondSpeed);
	
	// Third layer movement
	InfiniteScrolling(&thirdTopPos, &thirdTopPos2, &thirdBotPos, &thirdBotPos2, &thirdTopLayer, &thirdBottomLayer, thirdSpeed);

	return ret;
}

update_status ModuleLevel2::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	
	// Render center layers
	if (!App->render->Blit(backgroundTexture, (int)centerPos.x, (int)centerPos.y, &centerLayer,centerSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)centerPos2.x, (int)centerPos2.y, &centerLayer, centerSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}

	// Render first layers
	if (!App->render->Blit(backgroundTexture, (int)firstTopPos.x, (int)firstTopPos.y, &firstTopLayer, firstSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstTopPos2.x, (int)firstTopPos2.y, &firstTopLayer, firstSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstBotPos.x, (int)firstBotPos.y, &firstBottomLayer, firstSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstBotPos2.x, (int)firstBotPos2.y, &firstBottomLayer, firstSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}

	// Render second layers
	if (!App->render->Blit(backgroundTexture, (int)secondTopPos.x, (int)secondTopPos.y, &secondTopLayer,secondSpeed )) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondTopPos2.x, (int)secondTopPos2.y, &secondTopLayer, secondSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondBotPos.x, (int)secondBotPos.y, &secondBottomLayer, secondSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondBotPos2.x, (int)secondBotPos2.y, &secondBottomLayer, secondSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}

	// Render third layers
	if (!App->render->Blit(backgroundTexture, (int)thirdTopPos.x, (int)thirdTopPos.y, &thirdTopLayer, thirdSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdTopPos2.x, (int)thirdTopPos2.y, &thirdTopLayer, thirdSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdBotPos.x, (int)thirdBotPos.y, &thirdBottomLayer, thirdSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdBotPos2.x, (int)thirdBotPos2.y, &thirdBottomLayer, thirdSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}

	return ret;
}

void ModuleLevel2::InfiniteScrolling(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2,SDL_Rect* topLayer,SDL_Rect* botLayer, float speed ) {
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
	
		if ((App->render->camera.x*speed) >= top2->x*SCREEN_SIZE && top->x * SCREEN_SIZE <= top2->x * SCREEN_SIZE) {
			top->x = top2->x + topLayer->w;
		}
		else if ((App->render->camera.x * speed) >= top->x * SCREEN_SIZE && top2->x * SCREEN_SIZE <= top->x * SCREEN_SIZE) {
			top2->x = top->x + topLayer->w ;
		}
	}
	
	top = nullptr;
	top2 = nullptr;
	bot = nullptr;
	bot2 = nullptr;
	topLayer = nullptr;
	botLayer = nullptr;
}

bool ModuleLevel2::CleanUp() {
	bool ret = true;

	activeTextures = 0;

	if (!App->textures->Unload(backgroundTexture)) {
		LOG("Error unloading background textue in lvl 2");
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
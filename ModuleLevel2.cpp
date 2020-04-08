#include "ModuleLevel2.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModulePlayer.h"
#include "ModuleRenderer.h"
#include "ModuleEnemies.h"

ModuleLevel2::ModuleLevel2(){}
ModuleLevel2::~ModuleLevel2(){}
bool ModuleLevel2::Start() {
	bool ret = true;
	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/ThunderStorm_SpriteSheet.png");
	//center rect
	centerLayer = { 0, 100, 256, 62 };
	
	//firstTop & firstBottom
	firstTopLayer = { 0,81,256,16 };
	firstBottomLayer = { 0,165,256,17 };

	//secondTop & secondBottom
	secondTopLayer = { 0,52,256,27 };
	secondBottomLayer = { 0,184,256,30 };
	//thirdTop & thirdTop
	thirdTopLayer = {0,0,256,45};
	thirdBottomLayer = {0,217,256,33};

	
//background position in screen
	//center
	centerPos = { 0,85 };
	centerPos2 = { (float)centerLayer.w,85 };
	centerSpeed = 0.5;
	
	//first
	firstTopPos = { 0,69};
	firstBotPos = { 0, 147};
	firstTopPos2 = { (float)firstTopLayer.w,69 };
	firstBotPos2 = { (float)firstBottomLayer.w,147};
	firstSpeed = 1.0;
	//second
	secondTopPos = { 0,42 };
	secondBotPos = { 0,164 };
	secondTopPos2 = { (float)secondTopLayer.w,42};
	secondBotPos2 = { (float)secondBottomLayer.w,164 };
	secondSpeed = 1.5;
	//third
	thirdTopPos = { 0,0 };
	thirdBotPos = { 0,(float)(SCREEN_HEIGHT - thirdBottomLayer.h) };
	thirdTopPos2 = { (float)thirdTopLayer.w,0 };
	thirdBotPos2 = { (float)thirdBottomLayer.w,(float)(SCREEN_HEIGHT-thirdBottomLayer.h) };
	thirdSpeed = 2.0;

	App->player->Enable();

	App->enemies->AddEnemy(ENEMY_TYPE::MEDIUMCAMOUFLAGEJET, 256, 80);
	
	return ret;
}
update_status ModuleLevel2::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;
	
	App->render->camera.x += 3;

	//center layers movement
	InfiniteScrolling(&centerPos, &centerPos2, nullptr, nullptr, &centerLayer, nullptr, centerSpeed);

	//first layers movement
	InfiniteScrolling(&firstTopPos, &firstTopPos2, &firstBotPos, &firstBotPos2, &firstTopLayer, &firstBottomLayer, firstSpeed);
	
	//second layer movement
	InfiniteScrolling(&secondTopPos, &secondTopPos2, &secondBotPos, &secondBotPos2, &secondTopLayer, &secondBottomLayer, secondSpeed);

	//third layer movement
	InfiniteScrolling(&thirdTopPos, &thirdTopPos2, &thirdBotPos, &thirdBotPos2, &thirdTopLayer, &thirdBottomLayer, thirdSpeed);
	
	
	
	return ret;
}
update_status ModuleLevel2::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	//Render center layers
	if (!App->render->Blit(backgroundTexture, (int)centerPos.x, (int)centerPos.y, &centerLayer,centerSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)centerPos2.x, (int)centerPos2.y, &centerLayer, centerSpeed)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	//renderFirstLayers
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
	//renderSecondLayers
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
	//renderThirdLayers
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
	
		if ((App->render->camera.x*speed) >= top2->x*SCREEN_SIZE && top->x * SCREEN_SIZE <= top2->x * SCREEN_SIZE){
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
	if (!App->textures->Unload(backgroundTexture)) {
		LOG("Error unloading background textue in lvl 2");
		ret = false;
	}
	return ret;
}
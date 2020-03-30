#include "ModuleLevel2.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModulePlayer.h"
#include "ModuleRenderer.h"

ModuleLevel2::ModuleLevel2(){}
ModuleLevel2::~ModuleLevel2(){}
bool ModuleLevel2::Start(){
	bool ret = true;
	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/ThunderStorm_SpriteSheet.png");
	App->render->camera.x = 0;
	//center rect
	centerLayer.w = 255;
	centerLayer.h = 62;
	centerLayer.x = 0;
	centerLayer.y = 100;
	//firstTop & firstBottom
	firstTopLayer.w = 255;
	firstTopLayer.h = 16;
	firstTopLayer.x = 0;
	firstTopLayer.y = 81;

	firstBottomLayer.w = 255;
	firstBottomLayer.h = 17;
	firstBottomLayer.x = 0;
	firstBottomLayer.y = 165;
	//secondTop & secondBottom
	secondTopLayer.w = 255;
	secondTopLayer.h = 27;
	secondTopLayer.x = 0;
	secondTopLayer.y = 52;

	secondBottomLayer.w = 255;
	secondBottomLayer.h = 30;
	secondBottomLayer.x = 0;
	secondBottomLayer.y = 184;
	//thirdTop & thirdTop
	thirdTopLayer.w = 255;
	thirdTopLayer.h = 45;
	thirdTopLayer.x = 0;
	thirdTopLayer.y = 0;

	thirdBottomLayer.w = 255;
	thirdBottomLayer.h = 30;
	thirdBottomLayer.x = 0;
	thirdBottomLayer.y = 217;
	
//background position in screen
	//center
	centerPos = { 0,(float)((SCREEN_HEIGHT / 2)-(centerLayer.h/2))+3 };
	centerPos2 = { (float)centerLayer.w,(float)((SCREEN_HEIGHT / 2) - (centerLayer.h / 2)) + 3 };
	centerSpeed = 0.5f;
	//first
	firstTopPos = { 0,(float)(centerPos.y-(firstTopLayer.h))};
	firstBotPos = { 0,(float)((centerPos.y) + (centerLayer.h)) };
	firstTopPos2 = { (float)firstTopLayer.w,(float)(centerPos.y - (firstTopLayer.h)) };
	firstBotPos2 = { (float)firstBottomLayer.w,(float)((centerPos.y)+ (centerLayer.h)) };
	firstSpeed = 1.0f;
	//second
	secondTopPos = { 0,(float)(firstTopPos.y-secondTopLayer.h) };
	secondBotPos = { 0,(float)((firstBotPos.y) + (firstBottomLayer.h)) };
	secondTopPos2 = { (float)secondTopLayer.w,(float)(firstTopPos.y - secondTopLayer.h)};
	secondBotPos2 = { (float)secondBottomLayer.w,(float)((firstBotPos.y + firstBottomLayer.h)) };
	secondSpeed = 1.5f;
	//third
	thirdTopPos = { 0,0 };
	thirdBotPos = { 0,(float)((secondBotPos.y) + (secondBottomLayer.h)-3.1) };
	thirdTopPos2 = { (float)thirdTopLayer.w,0 };
	thirdBotPos2 = { (float)thirdBottomLayer.w,(float)((secondBotPos.y) + (secondBottomLayer.h) - 3.1) };
	thirdSpeed = 2.0f;

	App->player->Enable();
	
	return ret;
}
update_status ModuleLevel2::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;
	
	//center layers movement
	Parallax(&centerPos, &centerPos2, nullptr, nullptr, &centerLayer, nullptr, centerSpeed);

	//first layers movement
	Parallax(&firstTopPos, &firstTopPos2, &firstBotPos, &firstBotPos2, &firstTopLayer, &firstBottomLayer, firstSpeed);
	
	
	//second layer movement
	Parallax(&secondTopPos, &secondTopPos2, &secondBotPos, &secondBotPos2, &secondTopLayer, &secondBottomLayer, secondSpeed);

	//third layer movement
	Parallax(&thirdTopPos, &thirdTopPos2, &thirdBotPos, &thirdBotPos2, &thirdTopLayer, &thirdBottomLayer, thirdSpeed);

	
	//Render center layers
	if (!App->render->Blit(backgroundTexture,(int)centerPos.x, (int)centerPos.y, &centerLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)centerPos2.x, (int)centerPos2.y, &centerLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	//renderFirstLayers
	if (!App->render->Blit(backgroundTexture, (int)firstTopPos.x, (int)firstTopPos.y, &firstTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstTopPos2.x, (int)firstTopPos2.y, &firstTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstBotPos.x, (int)firstBotPos.y, &firstBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)firstBotPos2.x, (int)firstBotPos2.y, &firstBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	//renderSecondLayers
	if (!App->render->Blit(backgroundTexture, (int)secondTopPos.x, (int)secondTopPos.y, &secondTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondTopPos2.x, (int)secondTopPos2.y, &secondTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondBotPos.x, (int)secondBotPos.y, &secondBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)secondBotPos2.x, (int)secondBotPos2.y, &secondBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	//renderThirdLayers
	if (!App->render->Blit(backgroundTexture, (int)thirdTopPos.x, (int)thirdTopPos.y, &thirdTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdTopPos2.x, (int)thirdTopPos2.y, &thirdTopLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdBotPos.x, (int)thirdBotPos.y, &thirdBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)thirdBotPos2.x, (int)thirdBotPos2.y, &thirdBottomLayer, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	return ret;
}

void ModuleLevel2::Parallax(fPoint* top, fPoint* top2, fPoint* bot, fPoint* bot2,SDL_Rect* topLayer,SDL_Rect* botLayer, float speed ) {
	if (bot != nullptr) {
		if (top->x < SCREEN_WIDTH || top2->x < SCREEN_WIDTH) {
			top->x -= speed;
			top2->x -= speed;
			bot->x -= speed;
			bot2->x -= speed;
		}
		if (top2->x <= 0 && top2->x >= -1) {
			top->x = top2->x + topLayer->w;
			bot->x = bot2->x + botLayer->w;

		}
		else if (top->x <= 0 && top->x >= -1) {
			top2->x = top->x + topLayer->w;
			bot2->x = bot->x + botLayer->w;
		}
	}
	else {
		if (top->x < SCREEN_WIDTH || top2->x < SCREEN_WIDTH) {
			top->x -= speed;
			top2->x -= speed;
			
		}
		if (top2->x <= 0 && top2->x >= -1) {
			top->x = top2->x + topLayer->w;
		

		}
		else if (top->x <= 0 && top->x >= -1) {
			top2->x = top->x + topLayer->w;
		
		}
	}
	
}
bool ModuleLevel2::CleanUp() {
	bool ret = true;
	if (!App->textures->Unload(backgroundTexture)) {
		LOG("Error unloading background textue in lvl 2");
		ret = false;
	}
	return ret;
}
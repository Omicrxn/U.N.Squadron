#include "ModuleLevel2.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleRenderer.h"

ModuleLevel2::ModuleLevel2(){}
ModuleLevel2::~ModuleLevel2(){}
bool ModuleLevel2::Start(){
	bool ret = true;
	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/ThunderStorm_SpriteSheet.png");

	//background rect
	background.w = 516;
	background.h = 330;
	background.x = 0;
	background.y = 145;
	//Upper Clouds rect
	upperClouds.w = 516;
	upperClouds.h = 109;
	upperClouds.x = 0;
	upperClouds.y = 0;
	//Lower Clouds rect
	lowerClouds.w = 516;
	lowerClouds.h = 76;
	lowerClouds.x = 0;
	lowerClouds.y = 510;
//background position in screen
	background_x =0;
	background_y = (SCREEN_HEIGHT / 2)-(background.h/2);
	background2_x = background_x+background.w;
	background2_y = (SCREEN_HEIGHT / 2) - (background.h/2);
	background_speed = 0.5f;

	//upperClouds position in screen
	upperClouds_x = upperClouds.x;
	upperClouds_y = upperClouds.y;
	upperClouds2_x = upperClouds.x+upperClouds.w;
	upperClouds2_y = upperClouds.y;
	upperClouds_speed = 1.0f;
	//lowerClouds position in screen
	lowerClouds_y = SCREEN_HEIGHT-lowerClouds.h;
	lowerClouds2_y = SCREEN_HEIGHT - lowerClouds.h;
	
	return ret;
}
update_status ModuleLevel2::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;
	if (background_x < SCREEN_WIDTH || background2_x < SCREEN_WIDTH) {
		background_x -= background_speed;
		background2_x -= background_speed;
		upperClouds_x -= upperClouds_speed;
		upperClouds2_x -= upperClouds_speed;
		/*background_x -= background_speed;
		background2_x -= background_speed;*/
	}
	if (background2_x <= 0 && background2_x >= -1) {
		background_x = background2_x + background.w;
	
	}
	else if (background_x <= 0 && background_x >= -1) {
		background2_x = background_x + background.w;
	
	}
	if (upperClouds2_x <= 0 && upperClouds2_x >= -1) {
	
		upperClouds_x = upperClouds2_x + upperClouds.w;
	}
	else if (upperClouds_x <= 0 && upperClouds_x >= -1) {
		
		upperClouds2_x = upperClouds_x + upperClouds.w;
	}

	//Render Background
	if (!App->render->Blit(backgroundTexture, (int)background_x, (int)background_y, &background, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)background2_x, (int)background2_y, &background, 0.7f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	//Render Clouds
	if (!App->render->Blit(backgroundTexture, (int)upperClouds_x, (int)upperClouds_y, &upperClouds, 1.0f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)upperClouds2_x, (int)upperClouds2_y, &upperClouds, 1.0f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)upperClouds_x, (int)lowerClouds_y, &lowerClouds, 1.0f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	if (!App->render->Blit(backgroundTexture, (int)upperClouds2_x, (int)lowerClouds2_y, &lowerClouds, 1.0f)) {
		LOG("Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		ret = update_status::UPDATE_ERROR;
	}
	
	return ret;
}
bool ModuleLevel2::CleanUp() {
	bool ret = true;
	if (!App->textures->Unload(backgroundTexture)) {
		LOG("Error unloading background textue in lvl 2");
		ret = false;
	}
	return ret;
}
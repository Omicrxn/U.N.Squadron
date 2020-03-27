#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleRenderer.h"

ModulePlayer::ModulePlayer() : Module() {

}

ModulePlayer::~ModulePlayer() {

}

bool ModulePlayer::Start() {
	bool ret = true;

	texture = App->textures->Load("Assets/sprites/enemies/squadronshet7.png");
	position.x = 10;
	position.y = 10;
	player.x = 256;
	player.y = 0;
	player.w = 128;
	player.h = 64;

	return ret;
}

bool ModulePlayer::Init() {
	bool ret = true;

	return ret;
}

update_status ModulePlayer::Update() {
	update_status ret = UPDATE_CONTINUE;

	if (!App->render->Blit(texture, position.x, position.y, &player)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	App->textures->Unload(texture);

	return ret;
}
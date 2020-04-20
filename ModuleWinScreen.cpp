#include "ModuleWinScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"
ModuleWinScreen::ModuleWinScreen(bool startEnabled) : Module(startEnabled) {
	//screen rect
	screen = { 0,0,256,256 };
}

ModuleWinScreen::~ModuleWinScreen() {}

bool ModuleWinScreen::Start() {
	bool ret = true;
	startTime = SDL_GetTicks();
	endTime = startTime + 3000;
	tex = App->textures->Load("Assets/sprites/menus/WinScreen.png");

	if (tex == nullptr) {
		ret = false;
	}

	return ret;
}

update_status ModuleWinScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	actualTime = startTime + SDL_GetTicks();

	if (actualTime >= endTime && actualTime <= endTime+100) {
		App->transition->FadeToBlack(this, (Module*)App->startScreen,60);
	}
	return ret;
}

update_status ModuleWinScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit 
	if (!App->render->Blit(tex, 0, 0, &screen)) {
		ret = UPDATE_ERROR;
	}
	return ret;
}

bool ModuleWinScreen::CleanUp() {
	bool ret = true;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}

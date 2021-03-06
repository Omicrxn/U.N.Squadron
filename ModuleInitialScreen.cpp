#include "ModuleInitialScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"
#include "ModuleFonts.h"
#include "ModuleHUD.h"

ModuleInitialScreen::ModuleInitialScreen(bool startEnabled) : Module(startEnabled) {
	name = "Initial S";
	
	// Screen rect
	logo = { 0,0,256,224 };
	screen = { 0,0,SCREEN_WIDTH * SCREEN_SIZE,SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleInitialScreen::~ModuleInitialScreen() {}

bool ModuleInitialScreen::Start() {
	bool ret = true;

	startTime = SDL_GetTicks();
	actualTime = 0;
	endTime = 3000;

	logoTex = App->textures->Load("Assets/sprites/menus/Logo.png");
	++activeTextures; ++totalTextures;

	tex = App->textures->Load("Assets/sprites/menus/InitialScreen.png");
	++activeTextures; ++totalTextures;

	if (tex == nullptr) {
		ret = false;
	}

	return ret;
}

update_status ModuleInitialScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 60);
	}

	return ret;
}

update_status ModuleInitialScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	actualTime = SDL_GetTicks() - startTime;
	
	// Blit 
	if (actualTime < endTime) {
		if (!App->render->Blit(logoTex, 0, 0, &logo, 0.0f, false)) {
			ret = UPDATE_ERROR;
		}
	}
	else {
		if (!App->render->Blit(tex, 0, 0, &screen, 0.0f, false,true)) {
			ret = UPDATE_ERROR;
		}
	}
	
	return ret;
}

bool ModuleInitialScreen::CleanUp() {
	bool ret = true;

	startTime = 0;
	actualTime = 0;
	endTime = 0;

	activeTextures = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;
	if (!App->textures->Unload(logoTex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	return ret;
}
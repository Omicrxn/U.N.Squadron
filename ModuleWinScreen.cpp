#include "ModuleWinScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleStartScreen.h"
ModuleWinScreen::ModuleWinScreen(bool startEnabled) : Module(startEnabled) {
	name = "Win S";

	// Screen rect
	screen = { 0,0,256,224 };
}

ModuleWinScreen::~ModuleWinScreen() {}

bool ModuleWinScreen::Start() {
	bool ret = true;

	tex = App->textures->Load("Assets/sprites/menus/WinScreen.png");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	return ret;
}

update_status ModuleWinScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;
	
	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 60);
	}
	
	return ret;
}

update_status ModuleWinScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit 
	if (!App->render->Blit(tex, 0, 0, &screen, 1, false)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModuleWinScreen::CleanUp() {
	bool ret = true;

	activeTextures = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	return ret;
}

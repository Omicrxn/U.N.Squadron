#include "ModuleInitialScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"

ModuleInitialScreen::ModuleInitialScreen(bool startEnabled) : Module(startEnabled) {
	//screen rect
	screen = { 0,0,256,256 };
}

ModuleInitialScreen::~ModuleInitialScreen() {}

bool ModuleInitialScreen::Start() {
	bool ret = true;

	//startTime = SDL_GetTicks();
	//endTime = startTime + 3000;

	tex = App->textures->Load("Assets/sprites/menus/InitialScreen.png");

	if (tex == nullptr) {
		ret = false;
	}

	//Playing opening music
	//App->audio->PlayMusic("Assets/music/soundtrack/opening.ogg");

	return ret;
}

update_status ModuleInitialScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;
	
	/*actualTime = startTime + SDL_GetTicks();
	
	if (actualTime >= endTime && actualTime <= endTime + 100) {
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 90);
	}*/

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN)
	{
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 90);
	}
	
	return ret;
}

update_status ModuleInitialScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit unicorn
	if (!App->render->Blit(tex, 0, 0, &screen)) {
		ret = UPDATE_ERROR;
	}
	return ret;
}

bool ModuleInitialScreen::CleanUp() {
	bool ret = true;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	App->audio->StopMusic();

	return ret;
}
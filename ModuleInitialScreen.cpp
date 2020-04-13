#include "ModuleInitialScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleTransition.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"
ModuleInitialScreen::ModuleInitialScreen() {
	

	//screen rect
	screen = { 0,0,256,256 };

}
ModuleInitialScreen::~ModuleInitialScreen() {}

bool ModuleInitialScreen::Start() {
	startTime = SDL_GetTicks();
	endTime = startTime + 3000;
	bool ret = true;
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
	
	actualTime = startTime + SDL_GetTicks();
	
	 if (actualTime >= endTime && actualTime <= endTime+100) {
		 App->transition->TransitionStart(this, App->startScreen);

			}
	
	return ret;
}

update_status ModuleInitialScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	//blit unicorn
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
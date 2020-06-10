#include "ModuleLoseScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"

#include "ModuleLevel2.h"

ModuleLoseScreen::ModuleLoseScreen(bool startEnabled) : Module(startEnabled) {
	name = "Lose S";
}

ModuleLoseScreen::~ModuleLoseScreen() {}

bool ModuleLoseScreen::Start() {
	bool ret = true;

	anim.PushBack({ 0,0,256,224 });
	anim.PushBack({ 256,0,256,224 });
	anim.PushBack({ 512,0,256,224 });
	anim.PushBack({ 768,0,256,224 });
	anim.PushBack({ 1024,0,256,224 });
	anim.PushBack({ 1280,0,256,224 });
	anim.PushBack({ 1536,0,256,224 });
	anim.PushBack({ 1792,0,256,224 });
	anim.speed = 0.1f;

	tex = App->textures->Load("Assets/sprites/menus/LoseScreen.png");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	// Playing lose audio
	App->audio->PlayMusic("Assets/music/events/thankyouforplaying(youarecrazy).wav");

	return ret;
}

update_status ModuleLoseScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 60);
	}

	return ret;
}

update_status ModuleLoseScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit
	if (!App->render->Blit(tex, 0, 0, &anim.GetCurrentFrame(), 1, false)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModuleLoseScreen::CleanUp() {
	bool ret = true;

	activeTextures = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Loose Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->audio->StopMusic();

	return ret;
}
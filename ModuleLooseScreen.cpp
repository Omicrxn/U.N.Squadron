#include "ModuleLooseScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStartScreen.h"

#include "ModuleLevel2.h"
ModuleLooseScreen::ModuleLooseScreen(bool startEnabled) : Module(startEnabled) {

}

ModuleLooseScreen::~ModuleLooseScreen() {}

bool ModuleLooseScreen::Start() {
	bool ret = true;

	// Destroyed plane animation
	plane_anim.PushBack({ 27, 63, 192, 112 });
	plane_anim.PushBack({ 248, 63, 192, 112 });
	plane_anim.PushBack({ 26, 199, 192, 112 });
	plane_anim.PushBack({ 247, 200, 192, 112 });
	plane_anim.speed = 0.1f;

	// Game Over Rect
	gameOverTitle = { 21, 61, 196, 26 };

	// End Title Rect
	endTitle = { 225, 39, 24, 8};

	tex = App->textures->Load("Assets/sprites/menus/LooseScreen.png");
	if (tex == nullptr) {
		ret = false;
	}

	++activeTextures; ++totalTextures;

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	if (current_anim != &plane_anim) {
		current_anim = &plane_anim;
		plane_anim.Reset();
	}

	// Playing loose audio
	App->audio->PlayMusic("Assets/music/events/thankyouforplaying(youarecrazy).wav");

	return ret;
}

update_status ModuleLooseScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN) {
		
		App->transition->FadeToBlack(this, (Module*)App->startScreen, 60);
	}

	return ret;
}

update_status ModuleLooseScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	SDL_Rect rect = current_anim->GetCurrentFrame();

	// Blit destroyed plane
	if (!App->render->Blit(tex, 67, 45, &rect, 1, false)) {
		ret = UPDATE_ERROR;
	}

	// Blit Game Over
	if (!App->render->Blit(tex, 20, 18, &gameOverTitle, 1, false)) {
		ret = UPDATE_ERROR;
	}

	// Blit End Title
	if (!App->render->Blit(tex, 24, 97, &endTitle, 1, false)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModuleLooseScreen::CleanUp() {
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
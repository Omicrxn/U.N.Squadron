#include "ModuleStartScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"

#include "ModuleLevel1.h"

ModuleStartScreen::ModuleStartScreen(bool startEnabled) : Module(startEnabled) {
	name = "Start S";
}

ModuleStartScreen::~ModuleStartScreen() {}

bool ModuleStartScreen::Start() {
	bool ret = true;

	// Unicorn Animation
	unicorn_anim.PushBack({ 5,2,112,160 });
	unicorn_anim.PushBack({ 122,2,120,160 });
	unicorn_anim.PushBack({ 248,2,120,160 });
	unicorn_anim.speed = 0.1f;

	// title rect
	title = { 235,165,236,67 };

	// text rect
	text = { 2,168,216,128 };

	// selector rect
	selector.PushBack({ 236,240,13,16 });
	selector.PushBack({ 236,300,13,16 });
	selector.speed = 0;
	selector.Reset();


	tex = App->textures->Load("Assets/sprites/menus/Menu-Spritesheet.png");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;
	
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	selectorPos = {68,116};

	if (current_anim != &unicorn_anim) {
		current_anim = &unicorn_anim;
		unicorn_anim.Reset();
	}

	// Playing opening music
	App->audio->PlayMusic("Assets/music/soundtrack/opening.ogg");

	// Load choose option sound
	chooseFx = App->audio->LoadFx("Assets/music/events/chooseoption.wav");
	++activeFx; ++totalFx;

	// Load start event sound
	startFx = App->audio->LoadFx("Assets/music/events/start.wav");
	++activeFx; ++totalFx;

	return ret;
}

update_status ModuleStartScreen::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;

	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN || App->input->pads[0].l_y < 0.0f)
		&& selectorPos.y != 116) {
		selectorPos = { 68,116 };
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN || App->input->pads[0].l_y > 0.0f)
		&& selectorPos.y != 135) {
		selectorPos = { 68,135 };
		App->audio->PlayFx(0, 0);
	}
	
	if ((App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN && App->transition->hasEnded()) || App->input->pads[0].x == true) {
		
		switch (selectorPos.y) {
		case 116: {
			selector.speed = 0.2;
			App->audio->PlayFx(1, 0);
			App->transition->FadeToBlack(this, (Module*)App->selector, 60);
			
		} break;
		case 135: {

		} break;
		default:
			break;
		}
	}

	return ret;
}

update_status ModuleStartScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit unicorn
	if (!App->render->Blit(tex, 81, 50, &unicorn_anim.GetCurrentFrame(), 1, false)) {
		ret = UPDATE_ERROR;
	}

	// Blit title
	if (!App->render->Blit(tex, 11, 16, &title, 1, false)) {
		ret = UPDATE_ERROR;
	}

	// Blit text
	if (!App->render->Blit(tex, 24, 87, &text, 1, false)) {
		ret = UPDATE_ERROR;
	}

	// Blit selector
	if (!App->render->Blit(tex, selectorPos.x, selectorPos.y, &selector.GetCurrentFrame(), 1, false)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModuleStartScreen::CleanUp() {
	bool ret = true;

	activeTextures = activeFx = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->audio->UnloadFx(chooseFx);
	--totalFx;
	App->audio->UnloadFx(startFx);
	--totalFx;

	App->audio->StopMusic();
	
	return ret;
}
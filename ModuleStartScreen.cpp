#include "ModuleStartScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"

#include "ModuleLevel2.h"
ModuleStartScreen::ModuleStartScreen(){
	//Unicorn Animation
	unicorn_anim.PushBack({ 5,2,112,160 });
	unicorn_anim.PushBack({ 122,2,120,160 });
	unicorn_anim.PushBack({ 248,2,120,160 });
	unicorn_anim.speed = 0.1f;
	
	//Title rect
	title = { 235,165,236,67 };
	//text rect
	text = { 2,168,216,128 };
	//selector rect
	selector = { 236,240,13,16 };
}
ModuleStartScreen::~ModuleStartScreen(){}
bool ModuleStartScreen::Start(){
	bool ret = true;
	tex = App->textures->Load("Assets/sprites/menus/Menu-Spritesheet.png");
	if (tex == nullptr) {
		ret = false;
	}
	
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	selectorPos = {68,116};

	//Playing opening music
	App->audio->PlayMusic("Assets/music/soundtrack/opening.ogg");

	return ret;
}

update_status ModuleStartScreen::Update(){
	update_status ret = update_status::UPDATE_CONTINUE;

	if (current_anim != &unicorn_anim) {
		current_anim = &unicorn_anim;
		unicorn_anim.Reset();
	}

	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN)
		&& selectorPos.y != 116) {
		selectorPos = { 68,116 };
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN)
		&& selectorPos.y != 135) {
		selectorPos = { 68,135 };
	}
	
	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN) {
		switch (selectorPos.y)
		{
		case 116: {
			App->transition->FadeToBlack(this, App->lvl2,90);
		}break;
		case 135: {

		}break;
		default:
			break;
		}
	}

	return ret;
}

update_status ModuleStartScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	SDL_Rect rect = current_anim->GetCurrentFrame();
	//blit unicorn
	if (!App->render->Blit(tex, 81, 50, &rect)) {
		ret = UPDATE_ERROR;
	}
	//blit title
	if (!App->render->Blit(tex, 11, 16, &title)) {
		ret = UPDATE_ERROR;
	}
	//blit text
	if (!App->render->Blit(tex, 24, 87, &text)) {
		ret = UPDATE_ERROR;
	}
	//blit selector
	if (!App->render->Blit(tex, selectorPos.x, selectorPos.y, &selector)) {
		ret = UPDATE_ERROR;
	}
	return ret;
}

bool ModuleStartScreen::CleanUp(){
	bool ret = true;
	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	App->audio->StopMusic();
	
	return ret;
}
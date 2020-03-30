#include "ModuleStartScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleTransition.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"

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
	texture = App->textures->Load("Assets/sprites/menus/Menu-Spritesheet.png");
	if (texture == nullptr) {
		ret = false;
	}
	
	selectorPos = {68,116};

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
	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN){
		switch (selectorPos.y)
		{
		case 116: {
			App->transition->TransitionStart(this, App->lvl2);
		}break;
		case 135: {

		}break;
		default:
			break;
		}
	}


	SDL_Rect rect = current_anim->GetCurrentFrame();
	
	//blit unicorn
	App->render->Blit(texture, 81, 50, &rect);
	//blit title
	App->render->Blit(texture, 11, 16, &title);
	//blit text
	App->render->Blit(texture, 24, 87, &text);
	//blit selector
	App->render->Blit(texture, selectorPos.x, selectorPos.y, &selector);
	
	return ret;
}
bool ModuleStartScreen::CleanUp(){
	bool ret = true;
	App->textures->Unload(texture);
	return ret;
}
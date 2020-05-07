#include "ModuleStore.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleHUD.h"
#include <stdio.h>

#include "ModuleLevel2.h"

enum weapons {
	WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, WEAPON_5, WEAPON_6,
	WEAPON_7, WEAPON_8, WEAPON_9, WEAPON_10, WEAPON_11, EXIT
};

uint weapons[2][6] = { {WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, WEAPON_5, WEAPON_6},
					   {WEAPON_7, WEAPON_8, WEAPON_9, WEAPON_10, WEAPON_11, EXIT} };

uint rows = 0;
uint columns = 0;
uint weapon = weapons[rows][columns];

ModuleStore::ModuleStore(bool startEnabled) : Module(startEnabled) {
	name = "Store";
}

ModuleStore::~ModuleStore() {}

bool ModuleStore::Start() {
	bool ret = true;
	//Unicorn Animation
	/*unicorn_anim.PushBack({ 5,2,112,160 });
	unicorn_anim.PushBack({ 122,2,120,160 });
	unicorn_anim.PushBack({ 248,2,120,160 });
	unicorn_anim.speed = 0.1f;*/

	//title rect
	/*title = { 235,165,236,67 };*/
	//text rect
	/*text = { 2,168,216,128 };*/
	//selector rect
	/*selector = { 236,240,13,16 };*/

	//Background rect
	background = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	//Selector rect
	selector = { 209,82,54,64 };

	//Loading the background texture
	tex = App->textures->Load("Assets/sprites/menus/shop/store.png");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	//Loading the items texture
	tex2 = App->textures->Load("Assets/sprites/menus/shop/items.png");
	if (tex2 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	selectorPos = { 5,115 };

	/*if (current_anim != &unicorn_anim) {
		current_anim = &unicorn_anim;
		unicorn_anim.Reset();
	}*/

	//Playing opening music
	/*App->audio->PlayMusic("Assets/music/soundtrack/opening.ogg");*/

	//Loading the font to print text on screen
	yellowFont = App->fonts->Load("Assets/Fonts/FontY.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;
	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;

	rows = 0;
	columns = 0;

	return ret;
}

update_status ModuleStore::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	//Hanging the control of the selector
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN)
		&& selectorPos.y > 115 && rows > 0) {
		selectorPos.y -= 50;
		rows--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN)
		&& selectorPos.y < 165 && rows < 1) {
		selectorPos.y += 50;
		rows++;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN)
		&& selectorPos.x > 5 && columns > 0) {
		selectorPos.x -= 40;
		columns--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN)
		&& selectorPos.x < 200 && columns < 5) {
		selectorPos.x += 40;
		columns++;
		App->audio->PlayFx(0, 0);
	}

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN) {
		if (weapon == weapons[1][5])
		{
			App->transition->FadeToBlack(this, (Module*)App->lvl2, 60);
		}
	}

	weapon = weapons[rows][columns];

	return ret;
}

update_status ModuleStore::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	/*SDL_Rect rect = current_anim->GetCurrentFrame();*/

	//blit unicorn
	//if (!App->render->Blit(tex, 81, 50, &rect, 1, false)) {
	//	ret = UPDATE_ERROR;
	//}
	//blit title
	//if (!App->render->Blit(tex, 11, 16, &title, 1, false)) {
	//	ret = UPDATE_ERROR;
	//}
	//blit text
	//if (!App->render->Blit(tex, 24, 87, &text, 1, false)) {
	//	ret = UPDATE_ERROR;
	//}
	//blit selector
	//if (!App->render->Blit(tex, selectorPos.x, selectorPos.y, &selector, 1, false)) {
	//	ret = UPDATE_ERROR;
	//}

	//Blit background
	if (!App->render->Blit(tex, 0, 0, &background, 1, false))
		ret = UPDATE_ERROR;

	//Blit selector
	if (!App->render->Blit(tex2, selectorPos.x, selectorPos.y, &selector, 1, false))
		ret = UPDATE_ERROR;

	//Blit money
	sprintf_s(moneyText, 10, "%7d", App->HUD->money);
	strcat_s(moneyText, 10, "$");
	App->fonts->BlitText(5, 108, greenFont, moneyText);

	//Blit text
	App->fonts->BlitText(170, 50, yellowFont, "Select");
	App->fonts->BlitText(170, 60, yellowFont, "a weapon");

	//Blit money
	App->fonts->BlitText(5, 108, greenFont, moneyText);

	return ret;
}

bool ModuleStore::CleanUp() {
	bool ret = true;

	activeTextures = activeFonts = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	if (!App->textures->Unload(tex2)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->audio->StopMusic();

	return ret;
}
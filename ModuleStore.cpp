#include "ModuleStore.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleHUD.h"
#include "ModulePlayer.h"
#include "ModuleDebugInfo.h"

#include <stdio.h>

#include "ModuleLevel1.h"

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

	// Background rect
	background = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	// Selector rect
	selector = { 0,0,45,53 };

	// Loading the background texture
	tex = App->textures->Load("Assets/sprites/menus/shop/store.jpg");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	// Loading the items texture
	tex2 = App->textures->Load("Assets/sprites/menus/shop/selectionweapon.png");
	if (tex2 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	selectorPos = { 9,120 };

	// Load choose option sound
	chooseFx = App->audio->LoadFx("Assets/music/events/chooseoption.wav");
	++activeFx; ++totalFx;

	// Loading the font to print text on screen
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
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN) && rows > 0) {
		selectorPos.y -= 48;
		rows--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN) && rows < 1) {
		selectorPos.y += 48;
		rows++;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN) && columns > 0) {
		selectorPos.x -= 40;
		columns--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN) && columns < 5) {
		selectorPos.x += 40;
		columns++;
		App->audio->PlayFx(0, 0);
	}

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
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

	//Blit background
	if (!App->render->Blit(tex, 0, 0, &background, 1, false))
		ret = UPDATE_ERROR;

	//Blit selector
	if (!App->render->Blit(tex2, selectorPos.x, selectorPos.y, &selector, 1, false))
		ret = UPDATE_ERROR;

	//Blit money
	App->fonts->BlitText(17, 110, greenFont, "$");

	if (!App->debugInfo->maxMoney) sprintf_s(moneyText, 10, "%7d", App->player->money);
	else sprintf_s(moneyText, 10, "    MAX");

	App->fonts->BlitText(23, 110, greenFont, moneyText);

	//Blit text
	App->fonts->BlitText(170, 50, yellowFont, "Select");
	App->fonts->BlitText(170, 60, yellowFont, "a weapon");

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

	App->fonts->UnLoad(yellowFont);
	--totalFonts;

	App->fonts->UnLoad(greenFont);
	--totalFonts;

	App->audio->StopMusic();

	App->audio->UnloadFx(chooseFx);
	--totalFx;

	return ret;
}
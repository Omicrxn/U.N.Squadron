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
#include "ModuleWeapons.h";

#include <stdio.h>

// A few variables to manage the correct functioning of the store...
// Not necessary for the remaining modules (because of that they are not defined in the .h)
enum weapons {
	WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, SHELL, WEAPON_6,
	BOMB, WEAPON_8, WEAPON_9, WEAPON_10, WEAPON_11, EXIT
};

uint weapons[2][6] = { {WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, SHELL, WEAPON_6},
					   {BOMB, WEAPON_8, WEAPON_9, WEAPON_10, WEAPON_11, EXIT} };

uint rows = 0;
uint columns = 0;
uint weapon = weapons[rows][columns];
///////////////////////////////////////////////////////////////////////////////////////////

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

	// Already selected rect
	alreadySelected = { 0,0,33,41 };

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

	// Loading the already selected texture
	tex3 = App->textures->Load("Assets/sprites/menus/shop/alreadyselected.png");
	if (tex3 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	selectorPos = { 9,120 };

	// Load choose option sound
	chooseFx = App->audio->LoadFx("Assets/music/events/chooseoption.wav");
	++activeFx; ++totalFx;

	// Loading the font to print text on screen
	greyFont = App->fonts->Load("Assets/Fonts/FontW.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;
	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;

	rows = 0;
	columns = 0;

	// Resetting the weapons selected in the previous game
	weaponSelection = 0;

	return ret;
}

update_status ModuleStore::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	// Hanging the control of the selector
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN || App->input->pads[0].l_y < 0.0f) && rows > 0) {
		selectorPos.y -= 48;
		rows--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN || App->input->pads[0].l_y > 0.0f) && rows < 1) {
		selectorPos.y += 48;
		rows++;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN || App->input->pads[0].l_x < 0.0f) && columns > 0) {
		selectorPos.x -= 40;
		columns--;
		App->audio->PlayFx(0, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN || App->input->pads[0].l_x > 0.0f) && columns < 5) {
		selectorPos.x += 40;
		columns++;
		App->audio->PlayFx(0, 0);
	}

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].x == true) {
		// BOMB
		if (weapon == weapons[1][0] && App->player->money >= 2000) {
			if(!App->debugInfo->maxMoney)
				App->player->money -= 2000;
			weaponSelection |= (1 << 4);
		}

		// S.SHELL
		else if (weapon == weapons[0][4] && App->player->money >= 1000) {
			if (!App->debugInfo->maxMoney)
				App->player->money -= 1000;
			weaponSelection |= (1 << 6);
		}

		// EXIT
		if (weapon == weapons[1][5]) {
			App->transition->FadeToBlack(this, (Module*)App->lvl2, 60);
		}
	}

	weapon = weapons[rows][columns];

	return ret;
}

update_status ModuleStore::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit background
	if (!App->render->Blit(tex, 0, 0, &background, 1, false))
		ret = UPDATE_ERROR;

	// Blit selector
	if (!App->render->Blit(tex2, selectorPos.x, selectorPos.y, &selector, 1, false))
		ret = UPDATE_ERROR;

	// Blit money
	App->fonts->BlitText(17, 110, greenFont, "$");

	if (!App->debugInfo->maxMoney) sprintf_s(moneyText, 10, "%7d", App->player->money);
	else sprintf_s(moneyText, 10, "    MAX");

	App->fonts->BlitText(23, 110, greenFont, moneyText);

	// Blit text
	App->fonts->BlitText(170, 50, greyFont, "Select");
	App->fonts->BlitText(170, 60, greyFont, "a weapon");

	// Blit already selected texture
	// WEAPON_1
	if ((weaponSelection & (1 << 10)) != 0) {
		if (!App->render->Blit(tex3, 15, 126, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}
	// BOMB
	if ((weaponSelection & (1 << 4)) != 0) {
		if (!App->render->Blit(tex3, 15, 174, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}
	// SHELL
	if ((weaponSelection & (1 << 6)) != 0) {
		if (!App->render->Blit(tex3, 175, 126, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}

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

	if (!App->textures->Unload(tex3)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->fonts->UnLoad(greyFont);
	--totalFonts;

	App->fonts->UnLoad(greenFont);
	--totalFonts;

	App->audio->StopMusic();

	App->audio->UnloadFx(chooseFx);
	--totalFx;

	return ret;
}
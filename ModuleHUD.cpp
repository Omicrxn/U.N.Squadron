#include "ModuleHUD.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleDebugInfo.h"

#include <stdio.h>

ModuleHUD::ModuleHUD(bool startEnabled) : Module(startEnabled) {

	name = "HUD";
}

ModuleHUD::~ModuleHUD() {}

bool ModuleHUD::Start() {
	bool ret = true;

	App->player->score = 0;

	tex = App->textures->Load("Assets/sprites/hud/HUD.png");
	++activeTextures; ++totalTextures;

	//Animations
	fuelQuantity = {2,71,62,6};
	fuelBackground = {0,149,66,10};

	// Taking the bomb coordinates of the HUD spritesheet
	bomb = { 132,206,52,8 };

	// Taking the shell coordinates of the HUD spritesheet
	shell = { 132,84,69,11 };
	
	playerFace.PushBack({ 71,75,42,34 });
	powSquare = { 170,13,60,26 };
	helmet = { 16,168,15,14 };

	yellowFont = App->fonts->Load("Assets/Fonts/FontY.png", lookupTable, 5);
	++activeFonts; ++totalFonts;

	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", lookupTable, 5);
	++activeFonts; ++totalFonts;

	return ret;
}

update_status ModuleHUD::Update() {
	update_status ret = UPDATE_CONTINUE;
	
	fuelQuantity.w = 62 *App->player->GetCurrentFuel()/App->player->GetMaxFuel();

	return ret;
}

update_status ModuleHUD::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Draw UI (score & money)
	sprintf_s(scoreText, 10, "%7d", App->player->score);
	if (!App->debugInfo->maxMoney) sprintf_s(moneyText, 10, "%7d", App->player->money);
	else sprintf_s(moneyText, 10, "    MAX");

	sprintf_s(lifesText, 10, "%d", App->player->GetLifes());

	if (!App->debugInfo->debugMemLeaks && !App->debugInfo->debugGamepadInfo) {
		//Blit images
		App->render->Blit(tex, 71, 6, &playerFace.GetCurrentFrame(), 1, false);
		App->render->Blit(tex, 190, 15, &powSquare, 1, false);
		App->render->Blit(tex, 16, 201, &helmet, 1, false);
		App->render->Blit(tex, 55, 206, &fuelBackground, 1, false);
		App->render->Blit(tex, 57, 208, &fuelQuantity, 1, false);

		if (App->player->currentWeapon == App->player->weapons::BOMB) {
			App->render->Blit(tex, 132, 208, &bomb, 1, false);
			App->fonts->BlitText(224, 207, greenFont, "50");
		}
		else if (App->player->currentWeapon == App->player->weapons::SHELL)
		{
			App->render->Blit(tex, 132, 208, &shell, 1, false);
			App->fonts->BlitText(224, 207, greenFont, "50");
		}

		// Blit of the HUD
		App->fonts->BlitText(8, 15, yellowFont, "SCORE");
		App->fonts->BlitText(120, 15, yellowFont, "LEVEL");
		App->fonts->BlitText(121, 31, yellowFont, "$");
		App->fonts->BlitText(8, 31, greenFont, scoreText);
		App->fonts->BlitText(128, 31, greenFont, moneyText);
		App->fonts->BlitText(176, 15, greenFont, "2");
		App->fonts->BlitText(193, 31, greenFont, "max");
		App->fonts->BlitText(224, 31, greenFont, "  1");
		App->fonts->BlitText(33, 208, yellowFont, "=");
		App->fonts->BlitText(41, 207, greenFont, lifesText);
	}

	return ret;
}

bool ModuleHUD::CleanUp() {
	bool ret = true;

	activeTextures = activeFonts = 0;

	App->fonts->UnLoad(yellowFont);
	--totalFonts;
	App->fonts->UnLoad(greenFont);
	--totalFonts;

	App->textures->Unload(tex);
	--totalTextures;

	return ret;
}
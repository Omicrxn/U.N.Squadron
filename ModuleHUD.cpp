#include "ModuleHUD.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleDebugInfo.h"
#include "ModuleLevel1.h"

#include <stdio.h>

ModuleHUD::ModuleHUD(bool startEnabled) : Module(startEnabled) {

	name = "HUD";
	for (int i = 0; i < 12; ++i) {
		playerIn.PushBack ({42 * i, 0, 42, 34});
		playerOut.PushBack({ 42 * i, 34, 42, 34 });
	}
	playerOut.PushBack({ 0,0,0,0 });

	for (int i = 0; i < 8; ++i)
	{
		playerHasBeenHit.PushBack({ 42 * i, 68, 42, 34 });
	}
}

ModuleHUD::~ModuleHUD() {}

bool ModuleHUD::Start() {
	bool ret = true;

	App->player->score = 0;

	tex = App->textures->Load("Assets/sprites/hud/HUD.png");
	++activeTextures; ++totalTextures;

	tex2 = App->textures->Load("Assets/sprites/hud/CharacterStart.png");
	++activeTextures; ++totalTextures;

	// Animations
	fuelQuantity = {2,71,62,6};
	fuelBackground = {0,149,66,10};

	// Taking the bomb coordinates of the HUD spritesheet
	bomb = { 132,206,15,6 };

	// Taking the shell coordinates of the HUD spritesheet
	shell = { 132,83,21,12 };

	// Taking the falcon coordinates of the HUD spritesheet
	falcon = { 132,149,16,7 };

	// Taking the ceiling coordinates of the HUD spritesheet
	ceiling = { 132,176,15,9 };
	
	playerFace.PushBack({ 71,75,42,34 });
	powSquare = { 170,13,60,26 };
	helmet = { 16,168,15,14 };

	playerIn.speed = 0.25f;
	playerIn.loop = false;
	playerIn.Reset();

	playerOut.speed = 0.25f;
	playerOut.loop = false;
	playerOut.Reset();

	playerHasBeenHit.speed = 0.3f;
	playerHasBeenHit.loop = false;
	playerHasBeenHit.Reset();

	yellowFont = App->fonts->Load("Assets/Fonts/FontY.png", lookupTable, 5);
	++activeFonts; ++totalFonts;

	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", lookupTable, 5);
	++activeFonts; ++totalFonts;

	return ret;
}

update_status ModuleHUD::Update() {
	update_status ret = UPDATE_CONTINUE;
	
	fuelQuantity.w = 62 *App->player->GetCurrentFuel()/App->player->GetMaxFuel();

	// Draw HUD
	sprintf_s(scoreText, 10, "%7d", App->player->score);
	if (!App->debugInfo->maxMoney) sprintf_s(moneyText, 10, "%7d", App->player->money);
	else sprintf_s(moneyText, 10, "    max");

	sprintf_s(lifesText, 10, "%d", App->player->GetLifes());

	if(App->player->total >= 100) sprintf_s(totalText, 4, "max");
	else sprintf_s(totalText, 4, "%3d", App->player->total);

	sprintf_s(levelText, 2, "%1d", App->player->level);

	if (!App->player->maxPow) {
		if (App->player->level == 1) sprintf_s(powText, 4, "%3d", (4 - App->player->total));
		if (App->player->level == 2) sprintf_s(powText, 4, "%3d", (15 - App->player->total));
		if (App->player->level == 3) sprintf_s(powText, 4, "%3d", (26 - App->player->total));
		if (App->player->level == 4) sprintf_s(powText, 4, "%3d", (42 - App->player->total));
	}
	else sprintf_s(powText, 4, "max");

	// Passing the weapons ammo from uint to char
	if (!App->player->maxAmmo) {
		sprintf_s(falconAmmoText, 5, "%d", App->player->falconAmmo);
		sprintf_s(shellAmmoText, 5, "%d", App->player->shellAmmo);
		sprintf_s(bombAmmoText, 5, "%d", App->player->bombAmmo);
		sprintf_s(ceilingAmmoText, 5, "%d", App->player->ceilingAmmo);
	}
	else {
		sprintf_s(falconAmmoText, 5, "max");
		sprintf_s(shellAmmoText, 5, "max");
		sprintf_s(bombAmmoText, 5, "max");
		sprintf_s(ceilingAmmoText, 5, "max");
	}

	if (App->lvl1->IsEnabled())
	{
		sprintf_s(secondsText, 5, "%d", App->lvl1->seconds);
		sprintf_s(milisecondsText, 5, "%d", App->lvl1->miliseconds);
		sprintf_s(timer, 5, secondsText);
		strcat_s(timer, ":");
		strcat_s(timer, milisecondsText);
	}

	return ret;
}

update_status ModuleHUD::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	if (!App->debugInfo->debugMemLeaks && !App->debugInfo->debugGamepadInfo) {
		// Blit images
		
		App->render->Blit(tex, 190, 15, &powSquare, 1, false);
		App->render->Blit(tex, 16, 201, &helmet, 1, false);
		App->render->Blit(tex, 55, 206, &fuelBackground, 1, false);
		App->render->Blit(tex, 57, 208, &fuelQuantity, 1, false);

		if (playerIn.Finished() && App->player->GetCurrentFuel() > 0) {
			if (App->player->hasBeenHit)
			{
				if (!App->render->Blit(tex2, 71, 6, &playerHasBeenHit.GetCurrentFrame(), 1, false))
					ret = UPDATE_ERROR;
			}
			else
			{
				if (!App->render->Blit(tex, 71, 6, &playerFace.GetCurrentFrame(), 1, false))
					ret = UPDATE_ERROR;
			}
		}
		else if (playerIn.Finished() && App->player->GetCurrentFuel() == 0) {

			if (!App->render->Blit(tex2, 71, 6, &playerOut.GetCurrentFrame(), 1, false)) {
					ret = UPDATE_ERROR;
			}

		}
		else {
			if (!App->render->Blit(tex2, 71, 6, &playerIn.GetCurrentFrame(), 1, false)) {
				ret = UPDATE_ERROR;
			}
		}
		
		// Printing the current weapon and its price
		if (App->player->currentWeapon == App->player->weapons::FALCON)
		{
			App->render->Blit(tex, 132, 207, &falcon, 1, false);
			App->fonts->BlitText(155, 207, yellowFont, "Falcon");
			App->fonts->BlitText(224, 207, greenFont, falconAmmoText);
		}
		else if (App->player->currentWeapon == App->player->weapons::SHELL)
		{
			App->render->Blit(tex, 129, 204, &shell, 1, false);
			App->fonts->BlitText(155, 207, yellowFont, "S.Shell");
			App->fonts->BlitText(224, 207, greenFont, shellAmmoText);
		}
		else if (App->player->currentWeapon == App->player->weapons::BOMB) {
			App->render->Blit(tex, 132, 207, &bomb, 1, false);
			App->fonts->BlitText(155, 207, yellowFont, "Bomb");
			App->fonts->BlitText(224, 207, greenFont, bombAmmoText);
		}
		else if (App->player->currentWeapon == App->player->weapons::CEILING)
		{
			App->render->Blit(tex, 132, 207, &ceiling, 1, false);
			App->fonts->BlitText(155, 207, yellowFont, "Ceiling");
			App->fonts->BlitText(224, 207, greenFont, ceilingAmmoText);
		}

		// Blit of the HUD
		App->fonts->BlitText(8, 15, yellowFont, "SCORE");
		App->fonts->BlitText(120, 15, yellowFont, "LEVEL");
		App->fonts->BlitText(121, 31, yellowFont, "$");
		App->fonts->BlitText(8, 31, greenFont, scoreText);
		App->fonts->BlitText(128, 31, greenFont, moneyText);
		App->fonts->BlitText(176, 15, greenFont, levelText);
		App->fonts->BlitText(193, 31, greenFont, powText);
		App->fonts->BlitText(224, 31, greenFont, totalText);
		App->fonts->BlitText(32, 208, yellowFont, "=");
		App->fonts->BlitText(41, 207, greenFont, lifesText);

		if (App->lvl1->IsEnabled())
		{
			App->fonts->BlitText(108, SCREEN_HEIGHT/3, yellowFont, timer);
		}
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

	App->textures->Unload(tex2);
	--totalTextures;

	return ret;
}
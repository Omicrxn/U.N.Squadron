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

	score = 0;
	money = 0;
	
	tex = App->textures->Load("Assets/sprites/hud/HUD.png");
	++activeTextures; ++totalTextures;

	//Animations
	fuelQuantity = {2,71,62,6};
	fuelBackground = {0,149,66,10};
	/*fuel.PushBack({0,69,66,10});
	fuel.PushBack({0,79,66,10});
	fuel.PushBack({0,89,66,10});
	fuel.PushBack({0,99,66,10});
	fuel.PushBack({0,109,66,10});
	fuel.PushBack({0,119,66,10});
	fuel.PushBack({0,129,66,10});
	fuel.PushBack({0,139,66,10});
	fuel.PushBack({0,149,66,10});*/
	//fuel.speed = 0.06;
	weapon = { 132,206,52,8 };
	
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
	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(moneyText, 10, "%7d", money);
	sprintf_s(lifesText, 10, "%d", App->player->GetLifes());

	if (!App->debugInfo->debugMemLeaks) {
		//Blit images
		App->render->Blit(tex, 71, 6, &playerFace.GetCurrentFrame(), 1, false);
		App->render->Blit(tex, 190, 15, &powSquare, 1, false);
		App->render->Blit(tex, 16, 201, &helmet, 1, false);
		App->render->Blit(tex, 55, 206, &fuelBackground, 1, false);
		App->render->Blit(tex, 57, 208, &fuelQuantity, 1, false);
		App->render->Blit(tex, 132, 208, &weapon, 1, false);

		// Blit of the HUD (PROVISIONAL)
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
		App->fonts->BlitText(224, 207, greenFont, "50");
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
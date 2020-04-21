#include "ModuleHUD.h"
#include "Application.h"
#include "ModuleFonts.h"

#include <stdio.h>

ModuleHUD::ModuleHUD(bool startEnabled) : Module(startEnabled) {}

ModuleHUD::~ModuleHUD() {}

bool ModuleHUD::Start() {
	bool ret = true;
	// HUD (Prototype)
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = App->fonts->Load("Assets/Fonts/FontY.png", lookupTable, 5);
	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", lookupTable, 5);

	score = 0;
	money = 0;
	return ret;
}

update_status ModuleHUD::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Draw UI (score & money)
	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(moneyText, 10, "%7d", money);

	// Blit of the HUD (PROVISIONAL)
	App->fonts->BlitText(8, 10, yellowFont, "SCORE");
	App->fonts->BlitText(132, 10, yellowFont, "LEVEL");
	App->fonts->BlitText(132, 25, yellowFont, "$");
	App->fonts->BlitText(10, 25, greenFont, scoreText);
	App->fonts->BlitText(144, 25, greenFont, moneyText);
	App->fonts->BlitText(144, 10, greenFont, "      2");

	return ret;
}

bool ModuleHUD::CleanUp() {
	bool ret = true;

	App->fonts->UnLoad(yellowFont);
	App->fonts->UnLoad(greenFont);

	return ret;
}
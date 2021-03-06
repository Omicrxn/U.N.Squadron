#include "ModuleDebugInfo.h"

#include "Application.h"
#include "Globals.h"

#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleCollisions.h"
#include "ModuleTextureManager.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleHUD.h"
#include "ModulePlayer.h"
#include "ModuleLoseScreen.h"
#include "ModuleInitialScreen.h"
#include "ModuleFadeToBlack.h"
#include "ModuleStartScreen.h"
#include "ModuleLevel2.h"
#include "ModuleWinScreen.h"
#include "ModuleStore.h"
#include "ModuleSelector.h"
#include "ModuleLevel1.h"

#include "SDL2/include/SDL.h"
#include <stdio.h>

#define DebugModules 12

ModuleDebugInfo::ModuleDebugInfo(bool startEnabled) : Module(startEnabled) 
{
	name = "Debug Info";
}

ModuleDebugInfo::~ModuleDebugInfo() {}

bool ModuleDebugInfo::Start() 
{
	debugFont = App->fonts->Load("Assets/Fonts/FontY.png", App->HUD->lookupTable, 5);
	++totalFonts; ++activeFonts;

	return true;
}

bool ModuleDebugInfo::CleanUp() 
{
	// Unload debug font
	activeFonts = 0;

	App->fonts->UnLoad(debugFont);
	--totalFonts;

	return true;
}

update_status ModuleDebugInfo::Update() {

	// Mute Audio when F2 is pressed, press again to unmute
	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_DOWN) {
		App->audio->MuteMusic();
	}

	// Debug functionality to jump screens
	if (App->input->keyboard[SDL_SCANCODE_F4] == KEY_DOWN) {
		if (App->initialScreen->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->initialScreen, (Module*)App->startScreen, 30);
		}
		else if (App->startScreen->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->startScreen, (Module*)App->selector, 30);
		}
		else if (App->selector->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->selector, (Module*)App->store, 30);
		}
		else if (App->store->IsEnabled()) {
			if(App->selector->GetSelected()) App->transition->FadeToBlack((Module*)App->store, (Module*)App->lvl2, 30);
			else App->transition->FadeToBlack((Module*)App->store, (Module*)App->lvl1, 30);
		}
		else if (App->lvl2->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->selector, 30);
		}
		else if (App->lvl1->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->selector, 30);
		}
		else if (App->winScreen->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->winScreen, (Module*)App->selector, 30);
		}
		else if (App->loseScreen->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->loseScreen, (Module*)App->startScreen, 30);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_DOWN) {
		if (App->lvl2->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->startScreen, 30);
		}
		if (App->lvl1->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->startScreen, 30);
		}
	}

	// Debug functionality to Win Screen
	if (App->input->keyboard[SDL_SCANCODE_F6] == KEY_DOWN) {
		if (App->lvl2->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->winScreen, 30);
		}
		if (App->lvl1->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->winScreen, 30);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F7] == KEY_DOWN) {
		if (App->lvl2->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->loseScreen, 30);
		}
		if (App->lvl1->IsEnabled()) {
			App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->loseScreen, 30);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F8] == KEY_DOWN && !debugGamepadInfo) {
		debugMemLeaks = !debugMemLeaks;
	}

	if (debugMemLeaks) {
		if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN && counterModules <= DebugModules) {
			++counterModules;
		}

		if (counterModules == 1)
			inspectedModule = (Module*)App->initialScreen;
		else if (counterModules == 2)
			inspectedModule = (Module*)App->startScreen;
		else if (counterModules == 3)
			inspectedModule = (Module*)App->selector;
		else if (counterModules == 4)
			inspectedModule = (Module*)App->store;
		else if (counterModules == 5)
			inspectedModule = (Module*)App->lvl2;
		else if (counterModules == 6)
			inspectedModule = (Module*)App->lvl1;
		else if (counterModules == 7)
			inspectedModule = (Module*)App->player;
		else if (counterModules == 8)
			inspectedModule = (Module*)App->enemies;
		else if (counterModules == 9)
			inspectedModule = (Module*)App->weapons;
		else if (counterModules == 10)
			inspectedModule = (Module*)App->winScreen;
		else if (counterModules == 11)
			inspectedModule = (Module*)App->loseScreen;
		else if (counterModules == 12)
			inspectedModule = (Module*)App->debugInfo;

		if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN && counterModules == DebugModules) {
			counterModules = 0;
		}
	}

	// Switch gamepad debug info
	if (App->input->keyboard[SDL_SCANCODE_F10] == KEY_DOWN && !debugMemLeaks)
		debugGamepadInfo = !debugGamepadInfo;


	// Max Lifes
	if (App->input->keyboard[SDL_SCANCODE_8] == KEY_DOWN) {
		maxLifes = !maxLifes;
	}

	// Max Money
	if (App->input->keyboard[SDL_SCANCODE_9] == KEY_DOWN) {
		maxMoney = !maxMoney;
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleDebugInfo::PostUpdate() 
{
	if (debugMemLeaks) {
		App->fonts->BlitText(5, 5, debugFont, "Press F8 to close debug info");

		App->fonts->BlitText(5, 20, debugFont, "Total loaded resources");

		// Display total textures loaded
		sprintf_s(debugText, 150, "textures  %i", App->textures->GetTexturesCount());
		App->fonts->BlitText(20, 35, debugFont, debugText);

		// Display player's money
		if (maxMoney) App->fonts->BlitText(140, 35, debugFont, "Infinite Money");
		else {
			sprintf_s(debugText, 150, "Money  %i", App->player->money);
			App->fonts->BlitText(140, 35, debugFont, debugText);
		}
		// Display player's lifes
		if (maxLifes) App->fonts->BlitText(140, 50, debugFont, "Infinite Lifes");
		else {
			sprintf_s(debugText, 150, "Lifes  %i", App->player->GetLifes());
			App->fonts->BlitText(140, 50, debugFont, debugText);
		}

		if (App->player->maxAmmo) App->fonts->BlitText(140, 65, debugFont, "Infinite Ammo");
		else App->fonts->BlitText(140, 65, debugFont, "Limited Ammo");

		sprintf_s(debugText, 150, "Level  %i", App->player->level);
		App->fonts->BlitText(140, 80, debugFont, debugText);

		sprintf_s(debugText, 150, "Total  %i", App->player->total);
		App->fonts->BlitText(140, 95, debugFont, debugText);

		// Display total audio files loaded
		sprintf_s(debugText, 150, "audio fx  %i", App->audio->GetFxCount());
		App->fonts->BlitText(20, 50, debugFont, debugText);

		// Display total font files loaded
		sprintf_s(debugText, 150, "fonts     %i", App->fonts->GetFontsCount());
		App->fonts->BlitText(20, 65, debugFont, debugText);

		// Display total colliders loaded
		sprintf_s(debugText, 150, "colliders %i", App->collisions->GetColliderCount());
		App->fonts->BlitText(20, 80, debugFont, debugText);

		// Display total particles loaded
		sprintf_s(debugText, 150, "particles %i", App->particles->GetParticlesCount());
		App->fonts->BlitText(20, 95, debugFont, debugText);

		App->fonts->BlitText(5, 120, debugFont, "Press F9 to traverse modules");

		if (inspectedModule != nullptr) {
			DrawModuleResources(inspectedModule);
		}
	}
	else if (debugGamepadInfo)
		DrawGamepadInfo();

	return update_status::UPDATE_CONTINUE;
}

void ModuleDebugInfo::DrawModuleResources(Module* module) 
{
	sprintf_s(debugText, 150, "module %s", module->name);
	App->fonts->BlitText(5, 135, debugFont, debugText);

	sprintf_s(debugText, 150, "active  total");
	App->fonts->BlitText(150, 135, debugFont, debugText);

	sprintf_s(debugText, 150, "textures          %i      %i", module->activeTextures, module->totalTextures);
	App->fonts->BlitText(25, 150, debugFont, debugText);

	sprintf_s(debugText, 150, "audio fx          %i      %i", module->activeFx, module->totalFx);
	App->fonts->BlitText(25, 165, debugFont, debugText);

	sprintf_s(debugText, 150, "fonts             %i      %i", module->activeFonts, module->totalFonts);
	App->fonts->BlitText(25, 180, debugFont, debugText);

	sprintf_s(debugText, 150, "colliders         %i      %i", module->activeColliders, module->totalColliders);
	App->fonts->BlitText(25, 195, debugFont, debugText);
}

void ModuleDebugInfo::DrawGamepadInfo() {
	GamePad& pad = App->input->pads[0];

	sprintf_s(debugText, 150, "Pad 0 %s", (pad.enabled) ? "plugged" : "not detected");
	App->fonts->BlitText(5, 10, debugFont, debugText);

	sprintf_s(debugText, 150, "Buttons %s %s %s %s %s %s %s %s %s %s %s",
		(pad.a) ? "A" : "",
		(pad.b) ? "B" : "",
		(pad.x) ? "X" : "",
		(pad.y) ? "Y" : "",
		(pad.start) ? "START" : "",
		(pad.back) ? "BACK" : "",
		(pad.guide) ? "Guide" : "",
		(pad.l1) ? "LB" : "",
		(pad.r1) ? "RB" : "",
		(pad.l3) ? "L3" : "",
		(pad.r3) ? "R3" : ""
	);

	App->fonts->BlitText(5, 20, debugFont, debugText);

	sprintf_s(debugText, 150, "Dpad %s %s %s %s",
		(pad.up) ? "Up" : "",
		(pad.down) ? "Down" : "",
		(pad.left) ? "Left" : "",
		(pad.right) ? "Right" : ""
	);

	App->fonts->BlitText(5, 30, debugFont, debugText);

	sprintf_s(debugText, 150, "Left trigger  %0.2f", pad.l2);
	App->fonts->BlitText(5, 40, debugFont, debugText);

	sprintf_s(debugText, 150, "Right trigger %0.2f", pad.r2);
	App->fonts->BlitText(5, 50, debugFont, debugText);

	sprintf_s(debugText, 150, "Left thumb    %.2fx, %0.2fy", pad.l_x, pad.l_y);
	App->fonts->BlitText(5, 60, debugFont, debugText);

	sprintf_s(debugText, 150, "   Deadzone   %0.2f", pad.l_dz);
	App->fonts->BlitText(5, 70, debugFont, debugText);

	sprintf_s(debugText, 150, "Right thumb   %.2fx, %0.2fy", pad.r_x, pad.r_y);
	App->fonts->BlitText(5, 80, debugFont, debugText);

	sprintf_s(debugText, 150, "   Deadzone   %0.2f", pad.r_dz);
	App->fonts->BlitText(5, 90, debugFont, debugText);
}
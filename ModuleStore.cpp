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
#include "ModuleWeapons.h"
#include "ModuleSelector.h"

#include <stdio.h>

ModuleStore::ModuleStore(bool startEnabled) : Module(startEnabled) {
	name = "Store";
}

ModuleStore::~ModuleStore() {}

bool ModuleStore::Start() {
	bool ret = true;

	exitPressed = false;

	// Background rect
	background = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	// Selector rect
	selector = { 0,0,45,53 };

	// Already selected rect
	alreadySelected = { 0,0,33,41 };

	// Unicorn Animation
	talkStore.PushBack({ 0,0,72,119 });
	talkStore.PushBack({ 73,0,72,119 });
	talkStore.PushBack({ 219,0,72,119 });
	talkStore.speed = 0.01f;

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

	// Loading the talk store textures
	tex4 = App->textures->Load("Assets/sprites/menus/shop/talk store.png");
	if (tex4 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	selectorPos = { 9,120 };

	// Load choose option sound
	chooseFx = App->audio->LoadFx("Assets/music/events/chooseoption.wav");
	++activeFx; ++totalFx;

	boughtFx = App->audio->LoadFx("Assets/music/events/bought.wav");
	++activeFx; ++totalFx;

	noMoneyFx = App->audio->LoadFx("Assets/music/events/nomoney.wav");
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

	App->audio->PlayMusic("Assets/music/soundtrack/shop.ogg");

	return ret;
}

update_status ModuleStore::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	// Hanging the control of the selector
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN || App->input->pads[0].up == true) && rows > 0 && !exitPressed) {
		selectorPos.y -= 48;
		rows--;
		App->audio->PlayFx(chooseFx, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN || App->input->pads[0].down == true) && rows < 1 && !exitPressed) {
		selectorPos.y += 48;
		rows++;
		App->audio->PlayFx(chooseFx, 0);
	}
	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT || App->input->pads[0].left == true) && columns > 0 && !exitPressed) {
		if (CountdownL == 0) {
			selectorPos.x -= 40;
			columns--;
			App->audio->PlayFx(chooseFx, 0);

			CountdownL = MaxCountdownL;
		}
	}
	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || App->input->pads[0].right == true) && columns < 5 && !exitPressed) {
		if (CountdownR == 0) {
			selectorPos.x += 40;
			columns++;
			App->audio->PlayFx(chooseFx, 0);

			CountdownR = MaxCountdownR;
		}
	}

	// Handle the store state depending on the selector position
	if (currentState != BOUGHT && currentState != ALREADYBOUGHT && currentState != NOMONEY && currentState != BYE)
	{
		if (weapon == weapons[0][2])
		{
			currentState = FALCONSELECT;
		}
		else if (weapon == weapons[0][4])
		{
			currentState = SHELLSELECT;
		}
		else if (weapon == weapons[1][0])
		{
			currentState = BOMBSELECT;
		}
		else if (weapon == weapons[1][3])
		{
			currentState = CEILINGSELECT;
		}
		else if (weapon == weapons[1][5])
		{
			currentState = EXITSELECT;
		}
		else
		{
			currentState = CANTHANDLE;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
		// FALCON
		if (weapon == weapons[0][2]) {
			if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 8)) == 0) {
				if (App->player->money >= 9000) {
					App->player->money -= 9000;
					weaponSelection |= (1 << 8);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else {
					currentState = NOMONEY;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			else if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 8)) != 0)
			{
				currentState = ALREADYBOUGHT;
				App->audio->PlayFx(noMoneyFx, 0);
			}
			else if (App->debugInfo->maxMoney) {
				if ((weaponSelection & (1 << 8)) == 0)
				{
					weaponSelection |= (1 << 8);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else
				{
					currentState = ALREADYBOUGHT;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			storeStateCounter = 0;
		}

		// S.SHELL
		else if (weapon == weapons[0][4]) {
			if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 6)) == 0) {
				if (App->player->money >= 20000) {
					App->player->money -= 20000;
					weaponSelection |= (1 << 6);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else {
					currentState = NOMONEY;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			else if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 6)) != 0)
			{
				currentState = ALREADYBOUGHT;
				App->audio->PlayFx(noMoneyFx, 0);
			}
			else if (App->debugInfo->maxMoney) {
				if ((weaponSelection & (1 << 6)) == 0)
				{
					weaponSelection |= (1 << 6);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else
				{
					currentState = ALREADYBOUGHT;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			storeStateCounter = 0;
		}

		// BOMB
		if (weapon == weapons[1][0]) {
			if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 4)) == 0) {
				if (App->player->money >= 2000) {
					App->player->money -= 2000;
					weaponSelection |= (1 << 4);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else {
					currentState = NOMONEY;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			else if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 4)) != 0)
			{
				currentState = ALREADYBOUGHT;
				App->audio->PlayFx(noMoneyFx, 0);
			}
			else if (App->debugInfo->maxMoney) {
				if ((weaponSelection & (1 << 4)) == 0)
				{
					weaponSelection |= (1 << 4);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else
				{
					currentState = ALREADYBOUGHT;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			storeStateCounter = 0;
		}

		// CEILING
		else if (weapon == weapons[1][3]) {
			if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 2)) == 0) {
				if (App->player->money >= 15000) {
					App->player->money -= 15000;
					weaponSelection |= (1 << 2);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else {
					currentState = NOMONEY;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			else if (!App->debugInfo->maxMoney && (weaponSelection & (1 << 2)) != 0)
			{
				currentState = ALREADYBOUGHT;
				App->audio->PlayFx(noMoneyFx, 0);
			}
			else if (App->debugInfo->maxMoney) {
				if ((weaponSelection & (1 << 2)) == 0)
				{
					weaponSelection |= (1 << 2);
					currentState = BOUGHT;
					App->audio->PlayFx(boughtFx, 0);
				}
				else
				{
					currentState = ALREADYBOUGHT;
					App->audio->PlayFx(noMoneyFx, 0);
				}
			}
			storeStateCounter = 0;
		}

		// EXIT
		if (weapon == weapons[1][5]) {
			exitPressed = true;
			if(App->selector->GetSelected()) App->transition->FadeToBlack(this, (Module*)App->lvl2, 60);
			else App->transition->FadeToBlack(this, (Module*)App->lvl1, 60);
			currentState = BYE;
			storeStateCounter = 0;
		}
	}

	weapon = weapons[rows][columns];

	// Update selector countdown
	if (CountdownL > 0)
		--CountdownL;

	if (CountdownR > 0)
		--CountdownR;

	if (currentState == BOUGHT || currentState == ALREADYBOUGHT|| currentState == NOMONEY || currentState == BYE)
	{
		if (storeStateCounter < 100)
		{
			storeStateCounter++;
		}
	}
	
	if (storeStateCounter == 100)
	{
		storeStateCounter = 0;
		currentState = IDLE;
	}

	if (!App->debugInfo->maxMoney) sprintf_s(moneyText, 10, "%7d", App->player->money);
	else sprintf_s(moneyText, 10, "    MAX");

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
	App->fonts->BlitText(23, 110, greenFont, moneyText);

	// Blit text
	if (currentState == FALCONSELECT)
	{
		App->fonts->BlitText(160, 30, greyFont, "FALCON");
		App->fonts->BlitText(160, 40, greyFont, "");
		App->fonts->BlitText(160, 50, greyFont, "30pcs!");
	}
	else if (currentState == SHELLSELECT)
	{
		App->fonts->BlitText(160, 30, greyFont, "SUPER");
		App->fonts->BlitText(160, 40, greyFont, "   SHELL");
		App->fonts->BlitText(160, 50, greyFont, "");
		App->fonts->BlitText(160, 60, greyFont, "10pcs!");
	}
	else if (currentState == BOMBSELECT)
	{
		App->fonts->BlitText(160, 30, greyFont, "BOMB");
		App->fonts->BlitText(160, 40, greyFont, "");
		App->fonts->BlitText(160, 50, greyFont, "50pcs!");
	}
	else if (currentState == CEILINGSELECT)
	{
		App->fonts->BlitText(160, 30, greyFont, "SAILING");
		App->fonts->BlitText(160, 40, greyFont, "  MISSILE");
		App->fonts->BlitText(160, 50, greyFont, "");
		App->fonts->BlitText(160, 60, greyFont, "50pcs!");
	}
	else if (currentState == BOUGHT)
	{
		App->fonts->BlitText(160, 50, greyFont, "Thanks!");
	}
	else if (currentState == ALREADYBOUGHT)
	{
		App->fonts->BlitText(160, 40, greyFont, "You");
		App->fonts->BlitText(160, 50, greyFont, "already");
		App->fonts->BlitText(160, 60, greyFont, "have it!");
	}
	else if (currentState == NOMONEY)
	{
		App->fonts->BlitText(160, 40, greyFont, "You dont");
		App->fonts->BlitText(160, 50, greyFont, "have");
		App->fonts->BlitText(160, 60, greyFont, "enough");
		App->fonts->BlitText(160, 70, greyFont, "money...");
	}
	else if (currentState == CANTHANDLE)
	{
		App->fonts->BlitText(160, 40, greyFont, "Your");
		App->fonts->BlitText(160, 50, greyFont, "plane");
		App->fonts->BlitText(160, 60, greyFont, "cant");
		App->fonts->BlitText(160, 70, greyFont, "handle it!");
	}
	else if (currentState == EXITSELECT) {
		App->fonts->BlitText(160, 40, greyFont, "Are you");
		App->fonts->BlitText(160, 50, greyFont, "leaving");
		App->fonts->BlitText(160, 60, greyFont, "already?");
	}
	else if (currentState == BYE)
	{
		App->fonts->BlitText(160, 40, greyFont, "Thanks!");
		App->fonts->BlitText(160, 60, greyFont, "");
		App->fonts->BlitText(160, 70, greyFont, "You be");
		App->fonts->BlitText(160, 80, greyFont, "careful");
		App->fonts->BlitText(160, 90, greyFont, "now!");
	}

	// Blit already selected texture
	// FALCON
	if ((weaponSelection & (1 << 8)) != 0) {
		if (!App->render->Blit(tex3, 95, 126, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}
	// SHELL
	if ((weaponSelection & (1 << 6)) != 0) {
		if (!App->render->Blit(tex3, 175, 126, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}
	// BOMB
	if ((weaponSelection & (1 << 4)) != 0) {
		if (!App->render->Blit(tex3, 15, 174, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}
	// CEILING
	if ((weaponSelection & (1 << 2)) != 0) {
		if (!App->render->Blit(tex3, 135, 174, &alreadySelected, 1, false))
			ret = UPDATE_ERROR;
	}

	// Printing shopkeeper animation
	if (!App->render->Blit(tex4, 80, 0, &talkStore.GetCurrentFrame(), 1, false)) {
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

	if (!App->textures->Unload(tex4)) {
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
#include "ModuleLoseScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleHUD.h"
#include "ModuleStartScreen.h"

#include "ModuleLevel2.h"

ModuleLoseScreen::ModuleLoseScreen(bool startEnabled) : Module(startEnabled) {
	name = "Lose S";
}

ModuleLoseScreen::~ModuleLoseScreen() {}

bool ModuleLoseScreen::Start() {
	bool ret = true;

	continue3 = true;
	pressed = false;

	anim.PushBack({ 0,0,256,224 });
	anim.PushBack({ 256,0,256,224 });
	anim.PushBack({ 512,0,256,224 });
	anim.PushBack({ 768,0,256,224 });
	anim.PushBack({ 1024,0,256,224 });
	anim.PushBack({ 1280,0,256,224 });
	anim.PushBack({ 1536,0,256,224 });
	anim.PushBack({ 1792,0,256,224 });
	anim.PushBack({ 2048,0,256,224 });
	anim.PushBack({ 2304,0,256,224 });
	anim.PushBack({ 2560,0,256,224 });

	anim.speed = 0.1f;

	tex = App->textures->Load("Assets/sprites/menus/LoseScreen.png");
	if (tex == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	// Loading the font to print text on screen
	greyFont = App->fonts->Load("Assets/Fonts/FontW.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;

	greenFont = App->fonts->Load("Assets/Fonts/FontG.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;

	// Playing lose audio
	App->audio->PlayMusic("Assets/music/events/thankyouforplaying(youarecrazy).wav");

	loseFx = App->audio->LoadFx("Assets/music/events/start.wav");
	++activeFx; ++totalFx;

	return ret;
}

update_status ModuleLoseScreen::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	if (continue3) {
		if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN || App->input->pads[0].down == true) && !pressed) {
			continue3 = false;
		}
		else if ((App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) && App->transition->hasEnded()) {
			pressed = true;
			App->audio->PlayFx(loseFx, 0);
			App->transition->FadeToBlack(this, (Module*)App->selector, 90);
		}
	}
	else {
		if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN || App->input->pads[0].up == true) && !pressed) {
			continue3 = true;
		}
		else if ((App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) && App->transition->hasEnded()) {
			pressed = true;
			App->audio->PlayFx(loseFx, 0);
			App->transition->FadeToBlack(this, (Module*)App->initialScreen, 90);
		}
	}
	return ret;
}

update_status ModuleLoseScreen::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit
	if (!App->render->Blit(tex, 0, 0, &anim.GetCurrentFrame(), 0.0f, false)) {
		ret = UPDATE_ERROR;
	}

	if (continue3) {
		App->fonts->BlitText(96, 183, greenFont, "CONTINUE");
		App->fonts->BlitText(96, 199, greyFont, "END");
	}
	else {
		App->fonts->BlitText(96, 183, greyFont, "CONTINUE");
		App->fonts->BlitText(96, 199, greenFont, "END");
	}

	return ret;
}

bool ModuleLoseScreen::CleanUp() {
	bool ret = true;

	activeTextures = activeFonts = activeFx = 0;

	if (!App->textures->Unload(tex)) {
		LOG("Loose Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->fonts->UnLoad(greyFont);
	--totalFonts;

	App->fonts->UnLoad(greenFont);
	--totalFonts;

	App->audio->UnloadFx(loseFx);
	--totalFx;

	App->audio->StopMusic();

	return ret;
}
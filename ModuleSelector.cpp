#include "ModuleSelector.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleHUD.h"

ModuleSelector::ModuleSelector(bool startEnabled) : Module(startEnabled) {
	name = "Selector";
}

ModuleSelector::~ModuleSelector() {}

bool ModuleSelector::Start() {
	bool ret = true;

	selected = true;
	enterPressed = false;

	// Background rect
	background = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	tex1 = App->textures->Load("Assets/sprites/menus/Level1.png");
	if (tex1 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	tex2 = App->textures->Load("Assets/sprites/menus/Level2.png");
	if (tex2 == nullptr) {
		ret = false;
	}
	++activeTextures; ++totalTextures;

	greyFont = App->fonts->Load("Assets/Fonts/FontW.png", App->HUD->lookupTable, 5);
	++activeFonts; ++totalFonts;

	App->audio->PlayMusic("Assets/music/soundtrack/player_select.ogg");

	return ret;
}

update_status ModuleSelector::Update() {
	update_status ret = update_status::UPDATE_CONTINUE;

	if (!selected) {
		if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_DOWN || App->input->pads[0].left == true || App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_DOWN || App->input->pads[0].down == true) && !enterPressed) {
			selected = true;
		}
		else if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
			enterPressed = true;
			App->transition->FadeToBlack(this, (Module*)App->store, 60);
		}
	}
	else {
		if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_DOWN || App->input->pads[0].right == true || App->input->keyboard[SDL_SCANCODE_W] == KEY_DOWN || App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN || App->input->pads[0].up == true) && !enterPressed) {
			selected = false;
		}
		else if (App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_DOWN || App->input->pads[0].a == true) {
			enterPressed = true;
			App->transition->FadeToBlack(this, (Module*)App->store, 60);
		}
	}

	return ret;
}

update_status ModuleSelector::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	if (!selected) {
		if (!App->render->Blit(tex1, 0, 0, &background, 1, false))
			ret = UPDATE_ERROR;
		
		App->fonts->BlitText(80, 169, greyFont, "You selected");
		App->fonts->BlitText(160, 181, greyFont, "Level 1");
	}
	else {
		if (!App->render->Blit(tex2, 0, 0, &background, 1, false))
			ret = UPDATE_ERROR;

		App->fonts->BlitText(80, 169, greyFont, "You selected");
		App->fonts->BlitText(160, 181, greyFont, "Level 2");
	}

	return ret;
}

bool ModuleSelector::CleanUp() {
	bool ret = true;

	activeTextures = activeFonts = 0;

	if (!App->textures->Unload(tex1)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	if (!App->textures->Unload(tex2)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}
	--totalTextures;

	App->fonts->UnLoad(greyFont);
	--totalFonts;

	App->audio->StopMusic();

	return ret;
}
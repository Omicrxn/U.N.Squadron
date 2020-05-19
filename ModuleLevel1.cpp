#include "ModuleLevel1.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "ModuleHUD.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"

ModuleLevel1::ModuleLevel1(bool startEnabled) : Module(startEnabled) {
	name = "Level 1";
}

ModuleLevel1::~ModuleLevel1() {}

bool ModuleLevel1::Start() {
	bool ret = true;

	numSpawnedEnemies = 0;

	backgroundTexture = App->textures->Load("Assets/sprites/scenarios/Level 1.png");
	++activeTextures; ++totalTextures;

	// Music
	App->audio->PlayMusic("Assets/music/soundtrack/front-line-base.ogg");

	// Top Layer
	topLayer = { 4, 5, 254, 148 };

	// Bottom Layer before the boss
	firstBottomLayer = { 4, 157, 505, 157};
	secondBottomLayer = { 277, 157, 232, 157 };

	// Bottom Layer during the boss
	bossFirstBottomLayer = { 4, 237, 505, 76 };
	bossSecondBottomLayer = { 253, 237, 256, 76 };

	// Details of the level


	// Background and elements position
	return ret;
}
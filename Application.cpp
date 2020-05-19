#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTextureManager.h"
#include "ModuleLevel2.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleInitialScreen.h"
#include "ModuleWinScreen.h"
#include "ModuleStartScreen.h"
#include "ModuleLoseScreen.h"
#include "ModuleHUD.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleEnemies.h"
#include "ModuleFonts.h"
#include "ModuleStore.h"
#include "ModuleDebugInfo.h"
#include "ModuleWeapons.h"

Application::Application() {
	int i = 0;
	modules[i++] = window = new ModuleWindow(true);
	modules[i++] = input = new ModuleInput(true);
	modules[i++] = textures = new ModuleTextureManager(true);
	modules[i++] = audio = new ModuleAudio(true);

	
	modules[i++] = initialScreen = new ModuleInitialScreen(true);
	modules[i++] = startScreen = new ModuleStartScreen(false);
	modules[i++] = loseScreen = new ModuleLoseScreen(false);
	modules[i++] = store = new ModuleStore(false);
	modules[i++] = lvl2 = new ModuleLevel2(false);
	modules[i++] = winScreen = new ModuleWinScreen(false);
	

	modules[i++] = player = new ModulePlayer(false);
	modules[i++] = HUD = new ModuleHUD(false);
	modules[i++] = particles = new ModuleParticles(false);
	modules[i++] = enemies = new ModuleEnemies(false);
	modules[i++] = collisions = new ModuleCollisions(false);
	modules[i++] = weapons = new ModuleWeapons(false);
	

	modules[i++] = transition = new ModuleFadeToBlack(true);
	modules[i++] = fonts = new ModuleFonts(true);
	modules[i++] = debugInfo = new ModuleDebugInfo(true);
	modules[i++] = render = new ModuleRenderer(true);
}

Application::~Application() {
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

// INIT all modules
bool Application::Init() {
	bool ret = true;

	for (int i = 0; i < NUM_MODULES && ret == true; ++i) {
		ret = modules[i]->Init();
	}

	for (int i = 0; i < NUM_MODULES && ret == true; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;;
	}

	return ret;
}

update_status Application::Update() {
	
	frameStart = SDL_GetTicks();
	update_status ret = update_status::UPDATE_CONTINUE;
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : UPDATE_CONTINUE;
	}
	
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : UPDATE_CONTINUE;
	}
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : UPDATE_CONTINUE;
	}

	frameTime = SDL_GetTicks() - frameStart;
	if (frameDelay > frameTime) {
		SDL_Delay(frameDelay - frameTime);
	}

	return ret;
}

bool Application::CleanUp() {
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret == true; --i) {
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;
	}

	return ret;
}
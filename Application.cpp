#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTextureManager.h"
#include "ModuleLevel2.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleStartScreen.h"
#include "ModuleCollisions.h"
#include "ModuleTransition.h"
#include "ModuleRenderer.h"
#include "ModuleEnemies.h"

Application::Application()
{
	int i = 0;
	modules[i++] = window = new ModuleWindow();
	modules[i++] = input = new ModuleInput();
	modules[i++] = textures = new ModuleTextureManager();
	modules[i++] = audio = new ModuleAudio();

	modules[i++] = lvl2 = new ModuleLevel2();
	modules[i++] = player = new ModulePlayer();
	modules[i++] = enemies = new ModuleEnemies();
	modules[i++] = startScreen = new ModuleStartScreen();

	modules[i++] = particles = new ModuleParticles();
	modules[i++] = collisions = new ModuleCollisions();

	modules[i++] = transition = new ModuleTransition();
	modules[i++] = render = new ModuleRenderer();
}
Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
		
}

// INIT all modules
bool Application::Init()
{
	bool ret = true;
	startScreen->Disable();
	/*player->Disable();
	lvl2->Disable();*/

	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
	{
		ret = modules[i]->Init();
	}
	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
	{
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;;
	}

	return ret;
}

update_status Application::Update()
{
	
	frameStart = SDL_GetTicks();
	update_status ret = update_status::UPDATE_CONTINUE;
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : UPDATE_CONTINUE;
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : UPDATE_CONTINUE;
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : UPDATE_CONTINUE;
	}

	frameTime = SDL_GetTicks() - frameStart;
	if (frameDelay > frameTime) {
		SDL_Delay(frameDelay - frameTime);
	}
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret == true; --i) {
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;
	}
		
	return ret;
}
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleTransition.h"
#include "ModuleStartScreen.h"
#include "ModuleLevel2.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"

Application::Application()
{
	int i = 0;
	modules[i++] = window = new ModuleWindow();
	modules[i++] = render = new ModuleRenderer();
	modules[i++] = input = new ModuleInput();
	modules[i++] = textures = new ModuleTextureManager();
	modules[i++] = transition = new ModuleTransition();
	modules[i++] = startScreen = new ModuleStartScreen();
	modules[i++] = lvl2 = new ModuleLevel2();
	modules[i++] = player = new ModulePlayer();
}
Application::~Application()
{
	for (int i = NUM_MODULES - 1; i >= 0; --i) {
		delete modules[i];
	}
		
}

// INIT all modules
bool Application::Init()
{
	
	player->Disable();
	lvl2->Disable();
	bool ret = true;
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
	update_status ret = update_status::UPDATE_CONTINUE;
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : UPDATE_CONTINUE;
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : UPDATE_CONTINUE;
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : UPDATE_CONTINUE;
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
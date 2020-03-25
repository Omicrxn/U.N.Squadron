#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleTextureManager.h"
#include "ModuleInput.h"

Application::Application()
{
	int i = 0;
	modules[i++] = window = new ModuleWindow();
	modules[i++] = render = new ModuleRenderer();
	modules[i++] = input = new ModuleInput();
	modules[i++] = textures = new ModuleTextureManager();
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
	bool ret = true;
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		ret = modules[i]->Init();
	}

	return ret;
}

update_status Application::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;
	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->PreUpdate();
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->Update();
	}for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i) {
		ret = modules[i]->PostUpdate();
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret == true; --i) {
		ret = modules[i]->CleanUp();
	}
		
	return ret;
}
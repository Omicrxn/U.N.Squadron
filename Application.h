#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextureManager;
class ModuleAudio;
class ModulePlayer;
class ModuleLevel2;
class ModuleParticles;
class ModuleInitialScreen;
class ModuleStartScreen;
class ModuleFadeToBlack;
class ModuleCollisions;
class ModuleRenderer;
class ModuleEnemies;

#define NUM_MODULES 13

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();
	~Application();

	Module* modules[NUM_MODULES];
	ModuleWindow* window = nullptr;

	ModuleInput* input = nullptr;
	ModuleTextureManager* textures = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleLevel2* lvl2 = nullptr;
	ModulePlayer* player = nullptr;
	ModuleInitialScreen* initialScreen = nullptr;
	ModuleStartScreen* startScreen = nullptr;
	ModuleParticles* particles = nullptr;
	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack* transition = nullptr;
	ModuleRenderer* render = nullptr;
	ModuleEnemies* enemies = nullptr;

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	update_status Update();

	//Releases all the application data
	bool CleanUp();

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	UINT32 frameStart = 0;
	int frameTime;


};
//To make it global and be able to access from other calses without include
extern Application* App;
#endif // __APPLICATION_H__
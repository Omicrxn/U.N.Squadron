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
class ModuleStartScreen;
class ModuleTransition;
class ModuleCollisions;
class ModuleRenderer;

#define NUM_MODULES 11

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
	ModuleStartScreen* startScreen = nullptr;
	ModuleParticles* particles = nullptr;
	ModuleCollisions* collisions = nullptr;
	ModuleTransition* transition = nullptr;
	ModuleRenderer* render = nullptr;

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
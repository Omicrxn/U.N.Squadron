#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

class ModuleWindow;
class ModuleInput;
class ModuleRenderer;
class Module;
class ModuleTextureManager;
class ModuleLevel2;
class ModulePlayer;

#define NUM_MODULES 6

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();
	~Application();

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRenderer* render;
	ModuleInput* input;
	ModuleTextureManager* textures;
	ModuleLevel2* lvl2;
	ModulePlayer* player;
	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	update_status Update();

	//Releases all the application data
	bool CleanUp();



};
//To make it global and be able to access from other calses without include
extern Application* App;
#endif // __APPLICATION_H__
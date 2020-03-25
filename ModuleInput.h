#pragma once
#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_scancode.h"
#include "SDL.h"


#define MAX_KEYS 300

enum KEY_STATE {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();


	KEY_STATE keyboard[MAX_KEYS];

private:
	int controllerCharacterSelection;
	SDL_Event ev;

};

#endif // __ModuleInput_H__

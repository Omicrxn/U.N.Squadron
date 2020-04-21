#pragma once
#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_scancode.h"
#include "SDL.h"


#define MAX_KEYS 300

enum key_state {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:

	ModuleInput(bool startEnabled);
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();


	key_state keyboard[MAX_KEYS];

private:
	int controllerCharacterSelection;
	SDL_Event ev;
	bool maximized = false;

};

#endif // __ModuleInput_H__

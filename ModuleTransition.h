#pragma once
#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "Module.h"
#include "SDL.h"

class ModuleTransition : public Module
{
public:
	ModuleTransition();
	~ModuleTransition();

	bool Start();
	update_status PostUpdate();
	bool TransitionStart(Module* module_off, Module* module_on, float time = 2.0f);
	bool TransitionEnd();

private:

	float normalized;

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	Module* moduleOff;
	Module* moduleOn;
};

#endif //__TRANSITION_H__


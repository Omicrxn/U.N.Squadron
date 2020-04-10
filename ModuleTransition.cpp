#include "ModuleTransition.h"
#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "SDL.h"

ModuleTransition::ModuleTransition() {
	screen = { 0,0,SCREEN_WIDTH*SCREEN_SIZE,SCREEN_HEIGHT*SCREEN_SIZE };

}
ModuleTransition::~ModuleTransition() {
	
}

// Load assets
bool ModuleTransition::Start(){
	LOG("Transition Loading");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

update_status ModuleTransition::PostUpdate()
{
	if (current_step == fade_step::none)
		return UPDATE_CONTINUE;

	Uint32 now = SDL_GetTicks() - start_time;
	normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			this->moduleOff->Disable();
			this->moduleOn->Enable();
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{

		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	
	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return UPDATE_CONTINUE;
}




bool ModuleTransition::TransitionStart(Module* module_off, Module* module_on, float time)
{
	moduleOff = module_off;
	moduleOn = module_on;

	bool ret = false;
	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}
bool ModuleTransition::TransitionEnd() {
	bool ret = true;

	if (current_step == fade_step::none)
		ret = true;
	else
		ret = false;

	return ret;
}
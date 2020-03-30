#include "ModuleInput.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"

ModuleInput::ModuleInput() : Module()
{
	for (uint i = 0; i < MAX_KEYS; ++i)
		keyboard[i] = KEY_IDLE;
}
// Destructor
ModuleInput::~ModuleInput()
{
}

bool ModuleInput::Init()
{

	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	controllerCharacterSelection = 0;

	return ret;
}
update_status ModuleInput::PreUpdate() {
	SDL_PumpEvents();
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE) {
				keyboard[i] = KEY_DOWN;
			}
			else {
				keyboard[i] = KEY_REPEAT;
			}
		}
		else {
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) {
				keyboard[i] = KEY_UP;
			}
			else {
				keyboard[i] = KEY_IDLE;
			}
		}
	}
	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return update_status::UPDATE_STOP;
	}
	//CHECKS IF WINDOW X IS CLICKED TO CLOSE
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return update_status::UPDATE_STOP;
		}//Maximize window
		else if (event.type == SDL_WINDOWEVENT_MAXIMIZED) {
			SDL_MaximizeWindow(App->window->sdlWindow);

		}
	}
	return update_status:: UPDATE_CONTINUE;
}
// Called before quitting
bool ModuleInput::CleanUp()
{

	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
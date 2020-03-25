#include "ModuleWindow.h"
#include "Application.h"
#include "SDL.h"

ModuleWindow::ModuleWindow() : Module() {


}
ModuleWindow::~ModuleWindow() {


}

bool ModuleWindow::Init() {
	LOG("Init Window-----");

	bool ret = true;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO has not been initialized! SDL_Error: \n", SDL_GetError());
		ret = false;
	}
	else {
		windowIcon = SDL_LoadBMP(WINDOW_ICON_PATH);

		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (WIN_FULLSCREEN == true)
			flags |= SDL_WINDOW_FULLSCREEN;

		if (WIN_BORDERLESS == true)
			flags |= SDL_WINDOW_BORDERLESS;

		if (WIN_RESIZABLE == true)
			flags |= SDL_WINDOW_RESIZABLE;

		if (WIN_FULLSCREEN_DESKTOP == true)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		sdl_window = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		SDL_SetWindowIcon(sdl_window, windowIcon);
		if (sdl_window == NULL) {
			LOG("Window has not been created! SDL_Error: %s\n", SDL_GetError());
				ret = false;
		}
		else {
			screen_surface = SDL_GetWindowSurface(sdl_window);
		}
	}

	return ret;
}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (sdl_window != NULL)
		SDL_DestroyWindow(sdl_window);

	if (windowIcon != NULL)
		SDL_FreeSurface(windowIcon);

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}
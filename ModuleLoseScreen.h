#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL_timer.h"
#include "SDL_rect.h"
#include "Animation.h"

struct SDL_Texture;
class ModuleLoseScreen : public Module
{
public:
	ModuleLoseScreen(bool startEnabled);
	~ModuleLoseScreen();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* tex = nullptr;
	SDL_Rect screen;
	Animation plane_anim;
	Animation* current_anim = nullptr;
};
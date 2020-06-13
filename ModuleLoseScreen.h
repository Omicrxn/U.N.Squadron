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

	uint loseFx = 0;

private:
	SDL_Texture* tex = nullptr;
	Animation anim;
	Animation* current_anim = nullptr;

	// Id Fonts
	int greyFont = -1;
	int greenFont = -1;
	bool continue3;
	bool pressed = false;
};
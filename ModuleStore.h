#pragma once
#ifndef __MODULESTORE_H__
#define __MODULESTORE_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
class ModuleStore : public Module
{
public:
	ModuleStore(bool startEnabled);
	~ModuleStore();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	char moneyText[10] = { "\0" };

	// Id Fonts
	int yellowFont = -1;
	int greenFont = -1;

	// Sound effects indices
	uint chooseFx = 0;

private:
	SDL_Texture* tex = nullptr;
	SDL_Texture* tex2 = nullptr;
	SDL_Rect title;
	SDL_Rect text;
	SDL_Rect selector;
	SDL_Rect background;
	Animation unicorn_anim;
	Animation* current_anim = nullptr;
	iPoint selectorPos;
};
#endif // __MODULESTORE_H__
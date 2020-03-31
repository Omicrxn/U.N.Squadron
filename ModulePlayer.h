#pragma once
#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL_rect.h"

struct SDL_Texture;
class ModulePlayer : public Module {
public:
	ModulePlayer();
	~ModulePlayer();
	bool Start();
	bool Init();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	iPoint position;
	SDL_Texture* texture = nullptr;
	SDL_Rect player;
};

#endif // __MODULEPLAYER_H__
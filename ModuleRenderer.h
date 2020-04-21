#pragma once
#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "SDL_rect.h"

struct SDL_Texture;
struct SDL_Renderer;


class ModuleRenderer : public Module
{

public:
	ModuleRenderer(bool startEnabled);
	~ModuleRenderer();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, float speed = 1.0f, bool useCamera = true,bool initial = false);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float speed = 1.0f);
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;

};

#endif //__ModuleRenderer_H__
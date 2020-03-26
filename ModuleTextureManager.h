#pragma once
#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"
#include "Globals.h"
#define MAX_TEXTURES 200

struct SDL_Texture;
class ModuleTextureManager : public Module
{

public:
	ModuleTextureManager();
	~ModuleTextureManager();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	bool Unload(SDL_Texture* tex);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

	SDL_Texture* textures[MAX_TEXTURES];
	uint lastTexture = 0;
};

#endif //__ModuleTextures_H__
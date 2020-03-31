#include "ModuleTextureManager.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"
#include "SDL.h"
#include "SDL_image.h"

ModuleTextureManager::ModuleTextureManager() : Module() {
	for (uint i = 0; i < MAX_TEXTURES; i++)
	{
		textures[i] = nullptr;
	}
}

ModuleTextureManager::~ModuleTextureManager() {

}

bool ModuleTextureManager::Init() {
	LOG("Initializing Image Library");
	bool ret = true;

	//initializing PNG support
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);
	if ((init & flags) != flags) {
		LOG("There is an error initializing Img_Init: %s", IMG_GetError());
		ret = false;
	}
	return ret;
}

//Called before quitting
bool ModuleTextureManager::CleanUp() {
	LOG("Freeing textures and Image library");

	for (uint i = 0; i < MAX_TEXTURES; ++i)
		if (textures[i] != nullptr) {
			SDL_DestroyTexture(textures[i]);
			textures[i] = nullptr;
		}

	IMG_Quit();
	return true;
}

//Load new texture form file path
SDL_Texture* const ModuleTextureManager::Load(const char* path) {

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL) {
		LOG("Could not load surface from given path: %s. IMG_Load: %s", path, IMG_GetError);
	}
	else {
		texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);
		if (texture == NULL)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else {
			textures[lastTexture++] = texture;
			if (lastTexture > MAX_TEXTURES) {
				lastTexture = 0;
			}
		}
		SDL_FreeSurface(surface);


	}
	return texture;
}

bool ModuleTextureManager::Unload(SDL_Texture* texture) {
	bool ret = false;

	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		if (texture == textures[i]) {

			SDL_DestroyTexture(textures[i]);
			textures[i] = nullptr;
			
			ret = true;
			break;

		}
	}
	return ret;

}

// Retrieve size of a texture
void ModuleTextureManager::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}
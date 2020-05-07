#include "ModuleRenderer.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"

ModuleRenderer::ModuleRenderer(bool startEnabled) : Module(startEnabled) {
	name = "renderer";
	
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH*SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT*SCREEN_SIZE;
	
}

ModuleRenderer::~ModuleRenderer() {

}

bool ModuleRenderer::Init() {
	LOG("Creating Renderer-----");
	bool ret = true;
	Uint32 flags = 0;

	if (REN_VSYNC == true) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->sdlWindow, -1, flags);

	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE);
	
	if (renderer == NULL) {
		LOG("Error, renderer has not been created! The error is: %s\n",SDL_GetError());
		ret = false;
	}

	return ret;

}

update_status ModuleRenderer::PreUpdate() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update() {
	
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRenderer::PostUpdate() {
	
	SDL_RenderPresent(renderer);
	return update_status::UPDATE_CONTINUE;
}

bool ModuleRenderer::CleanUp() {
	LOG("Cleaning Up renderer -----");

	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);

	}
	return true;

}

// Blit to screen
bool ModuleRenderer::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, bool useCamera, bool initial)
{
	bool ret = true;

	SDL_Rect dstRect{ x * SCREEN_SIZE, y * SCREEN_SIZE, 0, 0 };

	if (useCamera)
	{
		dstRect.x -= (camera.x * speed);
		dstRect.y -= (camera.y * speed);
	}

	if (section != nullptr)
	{
		dstRect.w = section->w;
		dstRect.h = section->h;
	}
	else
	{
		//Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
	}
	if (!initial) {
		dstRect.w *= SCREEN_SIZE;
		dstRect.h *= SCREEN_SIZE;
	}


	if (SDL_RenderCopy(renderer, texture, section, &dstRect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
bool ModuleRenderer::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float speed)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect dstRect{
		(int)(-camera.x * speed) + rect.x * SCREEN_SIZE,
		(int)(-camera.y * speed) + rect.y * SCREEN_SIZE,
		rect.w * SCREEN_SIZE, rect.h * SCREEN_SIZE
	};

	if (SDL_RenderFillRect(renderer, &dstRect) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
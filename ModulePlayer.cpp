#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRenderer.h"
#include "ModuleStartScreen.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"

#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled) {

}

ModulePlayer::~ModulePlayer() {

}

bool ModulePlayer::Start() {
	bool ret = true;

	destroyed = false;

	//Loading the image into a texture
	texture = App->textures->Load("Assets/sprites/playablecharacters/player spaceships.png");

	//Position of the spaceship in the screen
	position.x = 10;
	position.y = SCREEN_HEIGHT / 2;

	//Position of the rectangle that we are getting from the image we load
	playerAnim.PushBack({ 155,187,32,9 });
	playerAnim.PushBack({ 190,186,32,12 });
	playerAnim.PushBack({ 120,185,32,16 });

	//Loading shooting sound effect
	App->audio->LoadFx("Assets/music/events/shoot.wav");

	//Loading collision sound effect
	App->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");

	// Add a collider to the player
	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	return ret;
}

bool ModulePlayer::Init() {
	bool ret = true;

	return ret;
}

update_status ModulePlayer::Update() {
	update_status ret = UPDATE_CONTINUE;

	// Moving the player with the camera scroll
	App->player->position.x += 1;
	
	//God Mode
	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_DOWN)
	{
		godMode = !godMode;
		if (godMode) {
			App->collisions->DeleteCollider(collider);
			collider = nullptr;
		}
		else {
			if (collider == nullptr) {
				collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);
			}
		}
	}
		
	// Spawn bullet particles when pressing SPACE
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN && !destroyed)
	{
		App->particles->AddParticle(App->particles->bullet, position.x + 32, position.y + 5, Collider::Type::PLAYER_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}

	// Moving the spaceship when pressing WASD
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_UP] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_IDLE)
	{
		if (current_anim != &playerAnim)
		{
			current_anim = &playerAnim;
		}
		rectAnim = current_anim->GetFrame(0);
	}
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
	{
		if (position.y > 37) {
			position.y -= 3;
			if (current_anim != &playerAnim)
			{
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(1);
		}
	}
	
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		if (position.y < SCREEN_HEIGHT - 18) {
			position.y += 3;
			if (current_anim != &playerAnim)
			{
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(2);
		}
		
	}
	
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
	{
		if (position.x > App->render->camera.x / SCREEN_SIZE) {
			position.x -= 3;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
	{
		if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
			position.x += 3;
		}
	}

	// Update collider position to player position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	if (position.x >= 1600) {
		App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->winScreen, 60);
	}

	return ret;
}

update_status ModulePlayer::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	
	// Blit player
	if (!destroyed)
	{
		if (!App->render->Blit(texture, position.x, position.y, &rectAnim)) {
			ret = UPDATE_ERROR;
		}
	}

	return ret;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	App->textures->Unload(texture);
	App->collisions->DeleteCollider(collider);

	collider = nullptr;
	
	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	// Detect collision with a bullet or an enemy. If so, disappear and explode.
	if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		
		App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->startScreen, 60);
	
		//Playing explosion sound effect
		App->audio->PlayFx(1, 0);

		destroyed = true;
	}
}
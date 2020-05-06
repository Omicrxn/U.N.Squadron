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
#include "ModuleHUD.h"

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

	GamePad& pad = App->input->pads[0];

	// Moving the player with the camera scroll
	App->player->position.x += 1;

	//Debug key for gamepad rumble testing purposes
	if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN)
	{
		App->input->ShakeController(0, 12, 0.33f);
	}

	//Debug key for gamepad rumble testing purposes
	if (App->input->keyboard[SDL_SCANCODE_2] == KEY_DOWN)
	{
		App->input->ShakeController(0, 36, 0.66f);
	}

	//Debug key for gamepad rumble testing purposes
	if (App->input->keyboard[SDL_SCANCODE_3] == KEY_DOWN)
	{
		App->input->ShakeController(0, 60, 1.0f);
	}
	
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
	
	
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		if (position.y < SCREEN_HEIGHT - 38) {
			position.y += 3;
			if (current_anim != &playerAnim)
			{
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(2);
		}
		
	}
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
	{
		if (position.y > 39) {
			position.y -= 3;
			if (current_anim != &playerAnim)
			{
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(1);
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

	if (position.x >= 7600) {
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
		if (currentFuel > 1) {
			currentFuel--;
		}
		else {
			playerLifes--;
			App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

			App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->startScreen, 60);

			//Playing explosion sound effect
			App->audio->PlayFx(1, 0);

			destroyed = true;
		}
		
	}
}

void ModulePlayer::DebugDrawGamepadInfo()
{
	GamePad& pad = App->input->pads[0];

	sprintf_s(App->HUD->scoreText, 150, "pad 0 %s, press 1/2/3 for rumble", (pad.enabled) ? "plugged" : "not detected");
	App->fonts->BlitText(5, 10, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "buttons %s %s %s %s %s %s %s %s %s %s %s",
		(pad.a) ? "a" : "",
		(pad.b) ? "b" : "",
		(pad.x) ? "x" : "",
		(pad.y) ? "y" : "",
		(pad.start) ? "start" : "",
		(pad.back) ? "back" : "",
		(pad.guide) ? "guide" : "",
		(pad.l1) ? "lb" : "",
		(pad.r1) ? "rb" : "",
		(pad.l3) ? "l3" : "",
		(pad.r3) ? "r3" : ""
	);
	App->fonts->BlitText(5, 20, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "dpad %s %s %s %s",
		(pad.up) ? "up" : "",
		(pad.down) ? "down" : "",
		(pad.left) ? "left" : "",
		(pad.right) ? "right" : ""
	);
	App->fonts->BlitText(5, 30, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "left trigger  %0.2f", pad.l2);
	App->fonts->BlitText(5, 40, App->HUD->yellowFont, App->HUD->scoreText);
	sprintf_s(App->HUD->scoreText, 150, "right trigger %0.2f", pad.r2);
	App->fonts->BlitText(5, 50, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "left thumb    %.2fx, %0.2fy", pad.l_x, pad.l_y);
	App->fonts->BlitText(5, 60, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "   deadzone   %0.2f", pad.l_dz);
	App->fonts->BlitText(5, 70, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "right thumb   %.2fx, %0.2fy", pad.r_x, pad.r_y);
	App->fonts->BlitText(5, 80, App->HUD->yellowFont, App->HUD->scoreText);

	sprintf_s(App->HUD->scoreText, 150, "   deadzone   %0.2f", pad.r_dz);
	App->fonts->BlitText(5, 90, App->HUD->yellowFont, App->HUD->scoreText);
}
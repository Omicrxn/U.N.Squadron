#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRenderer.h"
#include "ModuleStartScreen.h"
#include "ModuleLoseScreen.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModuleDebugInfo.h"
#include "ModuleWeapons.h"
#include "ModuleStore.h"

#include <stdio.h>

enum weapons {
	WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, SHELL, WEAPON_6,
	BOMB, WEAPON_8, WEAPON_9, WEAPON_10, WEAPON_11, EXIT
};

uint weaponCount;

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled) {
	name = "Player";
}

ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {
	bool ret = true;

	destroyed = false;

	currentFuel = maxFuel;

	// Loading the image into a texture
	texture = App->textures->Load("Assets/sprites/playablecharacters/player spaceships.png");
	++activeTextures; ++totalTextures;

	// Position of the spaceship in the screen
	position.x = 10;
	position.y = SCREEN_HEIGHT / 2;

	// Position of the rectangle that we are getting from the image we load
	playerAnim.PushBack({ 155,187,32,9 });
	playerAnim.PushBack({ 190,186,32,12 });
	playerAnim.PushBack({ 120,185,32,16 });

	// Loading shooting sound effect
	shootFx = App->audio->LoadFx("Assets/music/events/shoot.wav");
	++activeFx; ++totalFx;

	// Loading collision sound effect
	explosionFx = App->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");
	++activeFx; ++totalFx;

	// Add a collider to the player
	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);
	++activeColliders; ++totalColliders;

	this->weaponSelection = App->store->weaponSelection;

	if (weaponSelection == 0)
	{
		weaponCount == 0;
	}
	else
	{
		while ((weaponSelection & (1 << weaponCount)) == 0)
		{
			weaponCount++;
		}
	}

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

	// Spawn bullet particles when pressing SPACE or X
	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN && !destroyed) || pad.x == true) {
		App->particles->AddParticle(App->particles->bullet, position.x + 32, position.y + 5, Collider::Type::PLAYER_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(2, 0);
	}

	// Moving the spaceship when pressing WASD or using the Gamepad
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_UP] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_IDLE) {
		if (current_anim != &playerAnim) {
			current_anim = &playerAnim;
		}
		rectAnim = current_anim->GetFrame(0);
	}
	// If no up/down movement detected, set the current animation back to idle
	else if (pad.enabled) {
		if (pad.l_x == 0.0f && pad.l_y == 0.0f)
			current_anim = &playerAnim;
	}

	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || pad.l_y < 0.0f) {
		if (position.y > 39) {
			position.y -= 3;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(1);
		}
	}
	
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT || pad.l_x < 0.0f) {
		if (position.x > App->render->camera.x / SCREEN_SIZE) {
			position.x -= 3;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT || pad.l_y > 0.0f) {
		if (position.y < SCREEN_HEIGHT - 38) {
			position.y += 3;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(2);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || pad.l_y > 0.0f) {
		if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
			position.x += 3;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F] == KEY_DOWN) {
		switch (weaponCount)
		{
		case 4:
			if ((weaponSelection & (1 << 4)) != 0) {
				App->weapons->SpawnWeapon(WEAPON_TYPE::BOMB);
			}
			break;
		case 6:
			if ((weaponSelection & (1 << 6)) != 0) {
				App->weapons->SpawnWeapon(WEAPON_TYPE::SHELL);
			}
			break;
		default:
			break;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_C] == KEY_DOWN) {
		weaponCount++;
		while ((weaponSelection & (1 << weaponCount)) == 0)
		{
			weaponCount++;
		}
	}

	// Update collider position to player position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	// God Mode
	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_DOWN) {
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

	return ret;
}

update_status ModulePlayer::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	
	// Blit player
	if (!destroyed) {
		if (!App->render->Blit(texture, position.x, position.y, &rectAnim)) {
			ret = UPDATE_ERROR;
		}
	}

	return ret;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	activeTextures = activeColliders = activeFonts = activeFx = 0;

	App->textures->Unload(texture);
	--totalTextures;

	App->audio->UnloadFx(shootFx);
	--totalFx;
	App->audio->UnloadFx(explosionFx);
	--totalFx;

	App->collisions->DeleteCollider(collider);
	--totalColliders;
	collider = nullptr;
	
	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
	// Detect collision with a bullet or an enemy. If so, disappear and explode.
	if (c1 == collider && destroyed == false) {
		//SDL_SetTextureColorMod(texture, 450, 450, 64);
		if (currentFuel > 0) {
			currentFuel--;
		}
		else {
			App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

			if (playerLifes > 1 && !App->debugInfo->maxLifes) {
				playerLifes--;
				App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->startScreen, 60);
			}
			else {
				App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->loseScreen, 60);
			}

			//Playing explosion sound effect
			App->audio->PlayFx(3, 0);

			destroyed = true;
		}
	}
}
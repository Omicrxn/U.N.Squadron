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
#include "ModuleLevel1.h"
#include "ModuleLevel2.h"

#include <stdio.h>

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
	shootFx = App->audio->LoadFx("Assets/music/events/shoot1.wav");
	++activeFx; ++totalFx;

	// Loading collision sound effect
	explosionFx = App->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");
	++activeFx; ++totalFx;

	// Add a collider to the player
	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);
	++activeColliders; ++totalColliders;

	if (App->store->weaponSelection == 0)
	{
		currentWeapon = NONE;
		hasBought = false;
	}
	else
	{
		// 00000000 00000000 00000000 00000000
		//                          F  S B C
		weaponCount = 10;
		while ((App->store->weaponSelection & (1 << weaponCount)) == 0)
		{
			weaponCount--;
			if (weaponCount == 8)
			{
				currentWeapon = FALCON;
			}
			else if (weaponCount == 6)
			{
				currentWeapon = SHELL;
			}
			else if (weaponCount == 4)
			{
				currentWeapon = BOMB;
			}
			else if (weaponCount == 2)
			{
				currentWeapon = CEILING;
			}
		}
		hasBought = true;
	}

	falconAmmo = 30;
	shellAmmo = 10;
	bombAmmo = 50;
	ceilingAmmo = 50;

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
	if (App->lvl2->IsEnabled())
	{
		position.x++;
	}

	// Spawn bullet particles when pressing SPACE or X/R1
	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT && !destroyed) || pad.x == true || pad.r1 == true) {
		if (shotCountdown == 0) {
			if (level == 1) {
 				App->particles->AddParticle(App->particles->bullet1, position.x + 7, position.y + 5, Collider::Type::PLAYER_SHOT);
			}
			else if (level == 2) {
				App->particles->AddParticle(App->particles->bullet2, position.x + 7, position.y + 3, Collider::Type::PLAYER_SHOT);
			}
			else if (level == 3) {
				App->particles->AddParticle(App->particles->bullet3, position.x + 7, position.y + 1, Collider::Type::PLAYER_SHOT);
			}
			else if (level == 4) {
				App->particles->AddParticle(App->particles->bullet4, position.x + 7, position.y - 3, Collider::Type::PLAYER_SHOT);
			}

			// Playing shooting sound effect (if space was pressed)
			App->audio->PlayFx(0, 0);

			shotCountdown = shotMaxCountdown;
		}
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

	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || pad.l_y < 0.0f|| pad.up == true) {
		
		if (position.y > 42 && App->lvl2->IsEnabled()) {
			position.y -= 2;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(1);
		}
		else if (position.y > (App->render->camera.y / SCREEN_SIZE + 42) && App->lvl1->IsEnabled())
		{
			position.y -= 2;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(1);
		}
	}
	
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT || pad.l_x < 0.0f || pad.left == true) {
		if (position.x > App->render->camera.x / SCREEN_SIZE) {
			position.x -= 2;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT || pad.l_y > 0.0f || pad.down == true) {
		if (position.y < SCREEN_HEIGHT - 38 && App->lvl2->IsEnabled()) {
			position.y += 2;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(2);
		}
		else if (position.y < (App->render->camera.y / SCREEN_SIZE + SCREEN_HEIGHT - 38) && App->lvl1->IsEnabled())
		{
			position.y += 2;
			if (current_anim != &playerAnim) {
				current_anim = &playerAnim;
			}
			rectAnim = current_anim->GetFrame(2);
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || pad.l_y > 0.0f || pad.right == true) {
		if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
			position.x += 2;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F] == KEY_REPEAT || pad.b == true || pad.r2 == true) {
		if (weaponCountdown == 0) {
			switch (currentWeapon)
			{
			case FALCON:
				if ((App->store->weaponSelection & (1 << 8)) != 0 && falconAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::FALCON);
					if (!App->debugInfo->maxAmmo) falconAmmo--;
				}
				break;
			case SHELL:
				if ((App->store->weaponSelection & (1 << 6)) != 0 && shellAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::SHELL);
					if (!App->debugInfo->maxAmmo) shellAmmo--;
				}
				break;
			case BOMB:
				if ((App->store->weaponSelection & (1 << 4)) != 0 && bombAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::BOMB);
					if (!App->debugInfo->maxAmmo) bombAmmo--;
				}
				break;
			case CEILING:
				if ((App->store->weaponSelection & (1 << 2)) != 0 && ceilingAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::CEILING, position.x + 4, position.y - 4);
					App->particles->AddParticle(App->particles->ceilingExplosion, position.x, position.y - 4);
					if(!App->debugInfo->maxAmmo) ceilingAmmo--;
					ceilingCountdown = 60;
					// Playing shooting sound effect
					App->audio->PlayFx(0, 0);
				}
				break;
			default:
				break;
			}
			weaponCountdown = weaponMaxCountdown;
		}
	}

	if ((App->input->keyboard[SDL_SCANCODE_C] == KEY_REPEAT || pad.y == true || pad.l1 == true) && hasBought) {
		if (changeCountdown == 0) {
			weaponCount--;
			while ((App->store->weaponSelection & (1 << weaponCount)) == 0)
			{
				weaponCount--;
				if (weaponCount == 0)
				{
					weaponCount = 10;
				}
				else if (weaponCount == 8)
				{
					currentWeapon = FALCON;
				}
				else if (weaponCount == 6)
				{
					currentWeapon = SHELL;
				}
				else if (weaponCount == 4)
				{
					currentWeapon = BOMB;
				}
				if (weaponCount == 2)
				{
					currentWeapon = CEILING;
				}
			}
			changeCountdown = changeMaxCountdown;
		}
	}

	// Update collider position to player position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	// God Mode
	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN) {
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

	// Update shot countdown
	if (shotCountdown > 0)
		--shotCountdown;

	// Update weapon shot countdown
	if (weaponCountdown > 0)
		--weaponCountdown;

	// Update weapon change countdown
	if (changeCountdown > 0)
		--changeCountdown;

	if (ceilingCountdown > 0)
	{
		if (ceilingCountdown == 40)
		{
			App->weapons->SpawnWeapon(WEAPON_TYPE::CEILING, position.x + 8, position.y - 4);
			App->particles->AddParticle(App->particles->ceilingExplosion, position.x + 4, position.y - 4);
		}
		else if (ceilingCountdown == 20)
		{
			App->weapons->SpawnWeapon(WEAPON_TYPE::CEILING, position.x + 12, position.y - 4);
			App->particles->AddParticle(App->particles->ceilingExplosion, position.x + 8, position.y - 4);
		}
		ceilingCountdown--;
	}

	if (hasBeenHitCounter < 30)
	{
		hasBeenHitCounter++;
	}
	else if (hasBeenHitCounter == 30)
	{
		hasBeenHit = false;
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
		if (currentFuel > 1) {
			currentFuel--;
		}
		else {
			currentFuel--;
			App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

			if (playerLifes > 1 && !App->debugInfo->maxLifes) {
				playerLifes--;
				App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->startScreen, 60);
			}
			else {
				playerLifes = 3;
				money = 0;
				pow = 4;
				total = 0;
				level = 1;
				App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->loseScreen, 60);
			}

			destroyed = true;
		}

		hasBeenHit = true;
		hasBeenHitCounter = 0;

		//Playing explosion sound effect
		App->audio->PlayFx(2, 0);
	}
}
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
	playerAnimRight.PushBack({ 155,187,32,9 });
	playerAnimRight.PushBack({ 190,186,32,12 });
	playerAnimRight.PushBack({ 120,185,32,16 });

	playerAnimLeft.PushBack({ 48,187,32,9 });
	playerAnimLeft.PushBack({ 13,186,32,12 });
	playerAnimLeft.PushBack({ 83,185,32,16 });

	playerSpinningRight.PushBack({ 237,175,36,20 });
	playerSpinningRight.PushBack({ 289,174,28,22 });
	playerSpinningRight.PushBack({ 340,172,20,26 });
	playerSpinningRight.PushBack({ 391,170,18,30 });
	playerSpinningRight.PushBack({ 427,172,17,25 });
	playerSpinningRight.PushBack({ 469,174,27,21 });

	playerSpinningLeft.PushBack({ 242,216,28,23 });
	playerSpinningLeft.PushBack({ 285,215,19,25 });
	playerSpinningLeft.PushBack({ 333,213,15,29 });
	playerSpinningLeft.PushBack({ 377,216,19,24 });
	playerSpinningLeft.PushBack({ 422,216,27,23 });
	playerSpinningLeft.PushBack({ 467,218,35,20 });

	// Loading shooting sound effect
	shootFx = App->audio->LoadFx("Assets/music/events/shoot1.wav");
	++activeFx; ++totalFx;

	// Loading collision sound effect
	hitFx = App->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");
	++activeFx; ++totalFx;

	// Loading collision sound effect
	dieFx = App->audio->LoadFx("Assets/music/events/die.wav");
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
	if (damaged)
	{
		currentTime++;
		if (currentTime == 5)
		{
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			damaged = false;
			currentTime = 0;
		}


	}
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
				App->particles->AddParticle(App->particles->bullet3, position.x + 11, position.y, Collider::Type::PLAYER_SHOT);
			}
			else if (level == 4) {
				App->particles->AddParticle(App->particles->bullet4, position.x + 11, position.y - 3, Collider::Type::PLAYER_SHOT);
			}
			else if (level == 5) {
				App->particles->AddParticle(App->particles->bullet5, position.x + 10, position.y, Collider::Type::PLAYER_SHOT);
			}

			// Playing shooting sound effect (if space was pressed)
			App->audio->PlayFx(shootFx, 0);

			shotCountdown = shotMaxCountdown;
		}
	}

	// Moving the spaceship when pressing WASD or using the Gamepad
	if (App->lvl1->IsEnabled())
	{
		if (App->lvl1->spinningLeft || App->lvl1->spinningRight)
		{
			if (App->lvl1->spinningLeft)
			{
				if (App->lvl1->spinningLeftCounter <= 10)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(0);
				}
				else if (App->lvl1->spinningLeftCounter > 10 && App->lvl1->spinningLeftCounter <= 20)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(1);
				}
				else if (App->lvl1->spinningLeftCounter > 20 && App->lvl1->spinningLeftCounter <= 30)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(2);
				}
				else if (App->lvl1->spinningLeftCounter > 30 && App->lvl1->spinningLeftCounter <= 40)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(3);
				}
				else if (App->lvl1->spinningLeftCounter > 40 && App->lvl1->spinningLeftCounter <= 50)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(4);
				}
				else if (App->lvl1->spinningLeftCounter > 50 && App->lvl1->spinningLeftCounter <= 60)
				{
					if (current_anim != &playerSpinningLeft) {
						current_anim = &playerSpinningLeft;
					}
					rectAnim = current_anim->GetFrame(5);
				}
			}
			else if (App->lvl1->spinningRight)
			{
				if (App->lvl1->spinningRightCounter <= 10)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(0);
				}
				else if (App->lvl1->spinningRightCounter > 10 && App->lvl1->spinningRightCounter <= 20)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(1);
				}
				else if (App->lvl1->spinningRightCounter > 20 && App->lvl1->spinningRightCounter <= 30)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(2);
				}
				else if (App->lvl1->spinningRightCounter > 30 && App->lvl1->spinningRightCounter <= 40)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(3);
				}
				else if (App->lvl1->spinningRightCounter > 40 && App->lvl1->spinningRightCounter <= 50)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(4);
				}
				else if (App->lvl1->spinningRightCounter > 50 && App->lvl1->spinningRightCounter <= 60)
				{
					if (current_anim != &playerSpinningRight) {
						current_anim = &playerSpinningRight;
					}
					rectAnim = current_anim->GetFrame(5);
				}
			}

		}
		else
		{
			if (App->lvl1->currentDirection == App->lvl1->RIGHTDIR)
			{
				if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_UP] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_IDLE) {
					if (current_anim != &playerAnimRight) {
						current_anim = &playerAnimRight;
					}
					rectAnim = current_anim->GetFrame(0);
				}
				// If no up/down movement detected, set the current animation back to idle
				else if (pad.enabled) {
					if (pad.l_x == 0.0f && pad.l_y == 0.0f)
						current_anim = &playerAnimRight;
				}

				if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || pad.l_y < 0.0f || pad.up == true) {
					if (position.y > (App->render->camera.y / SCREEN_SIZE + 42) && App->lvl1->IsEnabled())
					{
						position.y -= 2;
						if (current_anim != &playerAnimRight) {
							current_anim = &playerAnimRight;
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
					if (position.y < (App->render->camera.y / SCREEN_SIZE + SCREEN_HEIGHT - 38) && App->lvl1->IsEnabled())
					{
						position.y += 2;
						if (current_anim != &playerAnimRight) {
							current_anim = &playerAnimRight;
						}
						rectAnim = current_anim->GetFrame(2);
					}
				}

				if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || pad.l_y > 0.0f || pad.right == true) {
					if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
						position.x += 2;
					}
				}
			}
			else if (App->lvl1->currentDirection == App->lvl1->LEFTDIR)
			{
				if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_UP] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_IDLE) {
					if (current_anim != &playerAnimLeft) {
						current_anim = &playerAnimLeft;
					}
					rectAnim = current_anim->GetFrame(0);
				}
				// If no up/down movement detected, set the current animation back to idle
				else if (pad.enabled) {
					if (pad.l_x == 0.0f && pad.l_y == 0.0f)
						current_anim = &playerAnimLeft;
				}

				if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || pad.l_y < 0.0f || pad.up == true) {
					if (position.y > (App->render->camera.y / SCREEN_SIZE + 42) && App->lvl1->IsEnabled())
					{
						position.y -= 2;
						if (current_anim != &playerAnimLeft) {
							current_anim = &playerAnimLeft;
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
					if (position.y < (App->render->camera.y / SCREEN_SIZE + SCREEN_HEIGHT - 38) && App->lvl1->IsEnabled())
					{
						position.y += 2;
						if (current_anim != &playerAnimLeft) {
							current_anim = &playerAnimLeft;
						}
						rectAnim = current_anim->GetFrame(2);
					}
				}

				if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || pad.l_y > 0.0f || pad.right == true) {
					if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
						position.x += 2;
					}
				}
			}
		}
	}
	else if (App->lvl2->IsEnabled())
	{
		if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_UP] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_IDLE) {
			if (current_anim != &playerAnimRight) {
				current_anim = &playerAnimRight;
			}
			rectAnim = current_anim->GetFrame(0);
		}
		// If no up/down movement detected, set the current animation back to idle
		else if (pad.enabled) {
			if (pad.l_x == 0.0f && pad.l_y == 0.0f)
				current_anim = &playerAnimRight;
		}

		if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || pad.l_y < 0.0f || pad.up == true) {

			if (position.y > 42 && App->lvl2->IsEnabled()) {
				position.y -= 2;
				if (current_anim != &playerAnimRight) {
					current_anim = &playerAnimRight;
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
				if (current_anim != &playerAnimRight) {
					current_anim = &playerAnimRight;
				}
				rectAnim = current_anim->GetFrame(2);
			}
		}

		if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || pad.l_y > 0.0f || pad.right == true) {
			if (position.x < (App->render->camera.x / SCREEN_SIZE + SCREEN_WIDTH - 32)) {
				position.x += 2;
			}
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F] == KEY_REPEAT || pad.b == true || pad.r2 == true) {
		if (weaponCountdown == 0) {
			switch (currentWeapon)
			{
			case FALCON:
				if ((App->store->weaponSelection & (1 << 8)) != 0 && falconAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::FALCON);
					if (!maxAmmo) falconAmmo--;
				}
				break;
			case SHELL:
				if ((App->store->weaponSelection & (1 << 6)) != 0 && shellAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::SHELL);
					if (!maxAmmo) shellAmmo--;
				}
				break;
			case BOMB:
				if ((App->store->weaponSelection & (1 << 4)) != 0 && bombAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::BOMB);
					if (!maxAmmo) bombAmmo--;
				}
				break;
			case CEILING:
				if ((App->store->weaponSelection & (1 << 2)) != 0 && ceilingAmmo > 0) {
					App->weapons->SpawnWeapon(WEAPON_TYPE::CEILING, position.x + 4, position.y - 4);
					App->particles->AddParticle(App->particles->ceilingExplosion, position.x, position.y - 4);
					if (!maxAmmo) ceilingAmmo--;
					ceilingCountdown = 60;
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

	// Max Ammo
	if (App->input->keyboard[SDL_SCANCODE_7] == KEY_DOWN) {
		maxAmmo = !maxAmmo;
	}

	// Change POW Level
	if (App->input->keyboard[SDL_SCANCODE_1] == KEY_DOWN) {
		level = 1;
	}
	if (App->input->keyboard[SDL_SCANCODE_2] == KEY_DOWN) {
		level = 2;
	}
	if (App->input->keyboard[SDL_SCANCODE_3] == KEY_DOWN) {
		level = 3;
	}
	if (App->input->keyboard[SDL_SCANCODE_4] == KEY_DOWN) {
		level = 4;
	}
	if (App->input->keyboard[SDL_SCANCODE_5] == KEY_DOWN) {
		level = 5;
		// maxPow = true;
	}

	if (total >= 4) level = 2;
	if (total >= 15) level = 3;
	if (total >= 26) level = 4;
	if (total >= 42) {
		level = 5;
		maxPow = true;
	}
	if (total >= 100) total = 100;

	if (App->input->keyboard[SDL_SCANCODE_N] == KEY_DOWN) {
		App->weapons->SpawnWeapon(WEAPON_TYPE::POWERUP_ORANGE, position.x + 40, position.y);
	}

	if (App->input->keyboard[SDL_SCANCODE_M] == KEY_DOWN) {
		App->weapons->SpawnWeapon(WEAPON_TYPE::POWERUP_BLUE, position.x + 40, position.y);
	}

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

	// Update shot countdown
	if (shotCountdown > 0)
		--shotCountdown;

	// Update weapon shot countdown
	if (weaponCountdown > 0)
		--weaponCountdown;

	// Update weapon change countdown
	if (changeCountdown > 0)
		--changeCountdown;

	if (collisionCountdown > 0) --collisionCountdown;

	return ret;
}

update_status ModulePlayer::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	// Blit player (depending on the level)
	//if (App->lvl1->IsEnabled()) {
	//	// To do
	//}
	//else if (App->lvl2->IsEnabled()) {
		if (!destroyed) {
			if (!App->render->Blit(texture, position.x, position.y, &rectAnim)) {
				ret = UPDATE_ERROR;
			}
		}
	//}

	return ret;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	activeTextures = activeFx = activeColliders = 0;

	App->textures->Unload(texture);
	--totalTextures;

	App->audio->UnloadFx(shootFx);
	--totalFx;
	App->audio->UnloadFx(hitFx);
	--totalFx;
	App->audio->UnloadFx(dieFx);
	--totalFx;

	App->collisions->DeleteCollider(collider);
	--totalColliders;
	collider = nullptr;

	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
	// Detect collision with a bullet or an enemy. If so, disappear and explode.
	if (c1 == collider && destroyed == false) {
		if (c2->type == Collider::Type::ENEMY_SHOT) {
			damaged = true;
			SDL_SetTextureColorMod(texture, 250, 200, 75);
			if (currentFuel > 1) {
				currentFuel--;

				//Playing hit sound effect
				App->audio->PlayFx(hitFx, 0);
			}
			else {
				currentFuel--;
				App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

				//Playing explosion sound effect
				App->audio->PlayFx(dieFx, 0);
				if (playerLifes > 1 && !App->debugInfo->maxLifes) {
					playerLifes--;
					if (App->lvl2->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->selector, 60);
					else if (App->lvl1->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->selector, 60);
				}
				else {
					playerLifes = 3;
					money = 3000;
					total = 0;
					level = 1;
					if (App->lvl2->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->loseScreen, 60);
					else if(App->lvl1->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->loseScreen, 60);
				}
				destroyed = true;
			}

			hasBeenHit = true;
			hasBeenHitCounter = 0;
		}
		else if (c2->type == Collider::Type::ENEMY) {
			if (collisionCountdown == 0) {
				if (currentFuel > 3) {
					currentFuel -= 3;

					//Playing hit sound effect
					App->audio->PlayFx(hitFx, 0);
				}
				else {
					currentFuel -= 3;
					App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

					//Playing explosion sound effect
					App->audio->PlayFx(dieFx, 0);
					if (playerLifes > 1 && !App->debugInfo->maxLifes) {
						playerLifes--;
						if (App->lvl2->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->selector, 60);
						else if (App->lvl1->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->selector, 60);
					}
					else {
						playerLifes = 3;
						money = 3000;
						total = 0;
						level = 1;
						if (App->lvl2->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl2, (Module*)App->loseScreen, 60);
						else if (App->lvl1->IsEnabled()) App->transition->FadeToBlack((Module*)App->lvl1, (Module*)App->loseScreen, 60);
					}
					destroyed = true;
				}

				hasBeenHit = true;
				hasBeenHitCounter = 0;
			}
			collisionCountdown = collisionMaxCountdown;
		}
		else if (c2->type == Collider::Type::POWERUP_ORANGE) {
			++total;
		}
		else if (c2->type == Collider::Type::POWERUP_BLUE) {
			total += 3;
		}
	}
}
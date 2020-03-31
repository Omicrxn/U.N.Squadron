#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRenderer.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer() : Module() {

}

ModulePlayer::~ModulePlayer() {

}

bool ModulePlayer::Start() {
	bool ret = true;

	//Loading the image into a texture
	texture = App->textures->Load("Assets/sprites/playablecharacters/player spaceships.png");

	//Position of the spaceship in the screen
	position.x = 10;
	position.y = 10;
	//Position of the rectangle that we are getting from the image we load
	player.x = 155;
	player.y = 187;
	player.w = 32;
	player.h = 9;

	//Loading shooting sound effect
	App->audio->LoadFx("Assets/music/events/shoot.wav");

	//Loading collision sound effect
	App->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");

	return ret;
}

bool ModulePlayer::Init() {
	bool ret = true;

	return ret;
}

update_status ModulePlayer::Update() {
	update_status ret = UPDATE_CONTINUE;

	// Spawn explosion particles when pressing B
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->bullet, position.x + 32, position.y, Collider::Type::PLAYER_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}

	//Moving the spaceship when pressing WASD
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT)
	{
		position.y -= 5;
	}
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		position.y += 5;
	}
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT)
	{
		position.x -= 5;
	}
	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT)
	{
		position.x += 5;
	}

	//Avoiding the spaceship to get out of the screen
	if (position.x < 0)
	{
		position.x = 0;

		//Playing collision sound effect (if we touch the border of the screen)
		App->audio->PlayFx(1, 0);
	}
	if (position.y <= 0)
	{
		position.y = 0;

		//Playing collision sound effect (if we touch the border of the screen)
		App->audio->PlayFx(1, 0);
	}
	if (position.x > SCREEN_WIDTH - 32)
	{
		position.x = SCREEN_WIDTH - 32;

		//Playing collision sound effect (if we touch the border of the screen)
		App->audio->PlayFx(1, 0);
	}
	if (position.y > SCREEN_HEIGHT - 9)
	{
		position.y = SCREEN_HEIGHT - 9;

		//Playing collision sound effect (if we touch the border of the screen)
		App->audio->PlayFx(1, 0);
	}

	return ret;
}

update_status ModulePlayer::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	if (!App->render->Blit(texture, position.x, position.y, &player)) {
		ret = UPDATE_ERROR;
	}

	return ret;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	App->textures->Unload(texture);

	return ret;
}
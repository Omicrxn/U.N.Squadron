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
	App->player->position.x += 0;
	
	//God Mode
	if (App->input->keyboard[SDL_SCANCODE_F5] == KEY_DOWN)
	{
		godMode = !godMode;
		if (godMode) {
			App->collisions->DeleteCollider(collider);
			collider = nullptr;
		}
		else {
			collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);
		}
	}
		
	// Spawn explosion particles when pressing B
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN && !destroyed)
	{
		App->particles->AddParticle(App->particles->bullet, position.x + 32, position.y, Collider::Type::PLAYER_SHOT);

		//Playing shooting sound effect (if space was pressed)
		App->audio->PlayFx(0, 0);
	}

	//Moving the spaceship when pressing WASD
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_IDLE && App->input->keyboard[SDL_SCANCODE_S] == KEY_IDLE)
	{
		if (current_anim != &playerAnim)
		{
			current_anim = &playerAnim;
		}
		rectAnim = current_anim->GetFrame(0);
	}
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_REPEAT)
	{
		position.y -= 5;
		if (current_anim != &playerAnim)
		{
			current_anim = &playerAnim;
		}
		rectAnim = current_anim->GetFrame(1);
	}
	
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		position.y += 5;
		if (current_anim != &playerAnim)
		{
			current_anim = &playerAnim;
		}
		rectAnim = current_anim->GetFrame(2);
	}
	
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT)
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_REPEAT)
	{
		position.x -= 5;
	}
	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_REPEAT)
	{
		position.x += 5;
	}

	// Update collider position to player position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	return ret;
}

update_status ModulePlayer::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;
	//blit player
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

	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	// TODO 5: Detect collision with a wall. If so, go back to intro screen.
	if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		

		//Playing explosion sound effect
		App->audio->PlayFx(1, 0);

		destroyed = true;
		
	}
}
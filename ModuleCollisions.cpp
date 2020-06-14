#include "ModuleCollisions.h"
#include "Application.h"

#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "SDL_scancode.h"

ModuleCollisions::ModuleCollisions(bool startEnabled) : Module(startEnabled) {
	name = "Collisions";

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::BOSS] = false;
	matrix[Collider::Type::WALL][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::WALL][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::WALL][Collider::Type::WEAPON] = true;
	matrix[Collider::Type::WALL][Collider::Type::WEAPON_SHELL] = true;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::BOSS] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::POWERUP_ORANGE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::POWERUP_BLUE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::BOSS] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::WEAPON] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::WEAPON_SHELL] = true;

	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::BOSS] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::BOSS] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::BOSS][Collider::Type::WALL] = true;
	matrix[Collider::Type::BOSS][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::BOSS][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::BOSS][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::BOSS][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::BOSS][Collider::Type::BOSS] = false;
	matrix[Collider::Type::BOSS][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::BOSS][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::BOSS][Collider::Type::WEAPON] = true;
	matrix[Collider::Type::BOSS][Collider::Type::WEAPON_SHELL] = true;

	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::BOSS] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::WALL] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::POWERUP_ORANGE][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::BOSS] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::WALL] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::POWERUP_BLUE][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::WEAPON][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::WEAPON][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::BOSS] = true;
	matrix[Collider::Type::WEAPON][Collider::Type::WALL] = true;
	matrix[Collider::Type::WEAPON][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::WEAPON] = false;
	matrix[Collider::Type::WEAPON][Collider::Type::WEAPON_SHELL] = false;

	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::POWERUP_ORANGE] = false;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::BOSS] = true;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::WALL] = true;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::POWERUP_BLUE] = false;
	matrix[Collider::Type::WEAPON_SHELL][Collider::Type::WEAPON] = false;
}

ModuleCollisions::~ModuleCollisions() {}

update_status ModuleCollisions::PreUpdate() {
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		// Skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// Avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
			// Skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->Intersects(c2->rect)) {
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update() {
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN)
		debug = !debug;

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate() {
	if (debug)
		DebugDraw();

	return update_status::UPDATE_CONTINUE;
}

void ModuleCollisions::DebugDraw() {
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type) {
		case Collider::Type::NONE: // White
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::WALL: // Blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::PLAYER: // Green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::ENEMY: // Red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::PLAYER_SHOT: // Yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case Collider::Type::ENEMY_SHOT: // Magenta
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case Collider::Type::BOSS: // Brown
			App->render->DrawQuad(colliders[i]->rect, 102, 51, 0, alpha);
			break;
		case Collider::Type::POWERUP_ORANGE: // Orange
			App->render->DrawQuad(colliders[i]->rect, 255, 128, 0, alpha);
			break;
		case Collider::Type::POWERUP_BLUE: // Blue Sky
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case Collider::Type::WEAPON: // GREY
			App->render->DrawQuad(colliders[i]->rect, 128, 128, 128, alpha);
			break;
		case Collider::Type::WEAPON_SHELL: // PINK
			App->render->DrawQuad(colliders[i]->rect, 127, 0, 255, alpha);
			break;
		}
	}
}

bool ModuleCollisions::CleanUp() {
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr) {
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener) {
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr) {
			ret = colliders[i] = new Collider(rect, type, listener);
			++colliderCount;
			break;
		}
	}

	return ret;
}

bool ModuleCollisions::DeleteCollider(Collider* collider) {
	if (collider != nullptr) {
		for (uint i = 0; i < MAX_COLLIDERS; ++i) {
			if (colliders[i] == collider) {
				collider->pendingToDelete = true;
				break;
			}
		}
	}

	return false;
}

// Collider class
Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener) {}

void Collider::SetPos(int x, int y) {
	rect.x = x;
	rect.y = y;
}

bool Collider::Intersects(const SDL_Rect& r) const {
	// Return true if there is an overlap
	// between argument "r" and property "rect"
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.y + rect.h > r.y);
}

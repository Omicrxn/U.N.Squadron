#pragma once
#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_scancode.h"
#include "SDL.h"

#define MAX_KEYS 300
#define MAX_PADS 4

struct _SDL_GameController;
struct _SDL_Haptic;

enum key_state {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct GamePad
{
	//Input data
	bool start, back, guide;
	bool x, y, a, b, l1, r1, l3, r3;
	bool up, down, left, right;
	float l2, r2;
	float l_x, l_y, r_x, r_y, l_dz, r_dz;

	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller;
	_SDL_Haptic* haptic;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;
};


class ModuleInput : public Module
{
public:

	ModuleInput(bool startEnabled);
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	// Activates SDL device funcionallity when a gamepad has been connected
	void HandleDeviceConnection(int index);

	// Deactivates SDL device funcionallity when a gamepad has been disconnected
	void HandleDeviceRemoval(int index);

	// Called at PreUpdate
	// Iterates through all active gamepads and update all input data
	void UpdateGamepadsInput();

	bool ShakeController(int id, int duration, float strength = 0.5f);
	const char* GetControllerName(int id) const;


	key_state keyboard[MAX_KEYS];

	// An array to fill in all detected gamepads
	GamePad pads[MAX_PADS];

private:
	bool maximized = false;
};

#endif // __ModuleInput_H__
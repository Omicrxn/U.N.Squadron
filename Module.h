#pragma once
#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

struct Collider;

class Module
{
private:
	bool enabled = true;
public:
	Module(bool startEnabled);

	//Called at the beginning of the application execution
	virtual bool Init();

	//Called when the module is activated
	virtual bool Start();

	//Called at the beginning of each application loop
	virtual update_status PreUpdate();

	//Called at the middle of each application loop
	virtual update_status Update();

	//Called at the end of each application loop
	virtual update_status PostUpdate();

	//Called at the end of the application
	virtual bool CleanUp();

	//Called when two colliders are intersecting
	//and the module is registered as the listener
	virtual void OnCollision(Collider* c1, Collider* c2);

	// Switches isEnabled and calls Start() method
	void Enable();

	// Switches isEnabled and calls CleanUp() method
	void Disable();

	inline bool IsEnabled() const { return enabled; }

public:
	const char* name = "None";

	// Resources info
	uint activeFonts = 0;
	uint totalFonts = 0;

	uint activeTextures = 0;
	uint totalTextures = 0;

	uint activeColliders = 0;
	uint totalColliders = 0;

	uint activeFx = 0;
	uint totalFx = 0;
};

#endif // __MODULE_H__
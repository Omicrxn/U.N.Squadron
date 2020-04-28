#pragma once

#ifndef __MODULE_HUD_H__
#define __MODULE_HUD_H__

#include "Module.h"

class ModuleHUD : public Module
{
public:
	ModuleHUD(bool startEnabled);

	~ModuleHUD();

	bool Start();

	update_status PostUpdate();

	bool CleanUp();

private:
	char scoreText[10] = { "\0" };
	char moneyText[10] = { "\0" };

public:
	// Id Fonts
	int yellowFont = -1;
	int greenFont = -1;

	// Score
	uint score = 0;

	// Money
	uint money = 0;
};

#endif // __ModuleHUD_H__
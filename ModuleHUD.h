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

public:
	// Score
	uint score = 0;
	char scoreText[10] = { "\0" };

	// Money
	uint money = 0;
	char moneyText[10] = { "\0" };

	// Id Fonts
	int yellowFont = -1;
	int greenFont = -1;
};

#endif // __ModuleHUD_H__
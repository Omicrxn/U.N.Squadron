#include "ModuleLooseScreen.h"
#include "Application.h"
#include "ModuleTextureManager.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"

#include "ModuleLevel2.h"
ModuleLooseScreen::ModuleLooseScreen(bool startEnabled) : Module(startEnabled) {

}

ModuleLooseScreen::~ModuleLooseScreen() {}

bool ModuleLooseScreen::Start() {
	bool ret = true;

	//Plane destroyed animation
	
}
#pragma once
#include "Module.h"
#include "Animation.h"
#include <string>

struct SDL_Texture;

class ModuleSceneMusicSelection :
	public Module
{
public:
	SDL_Texture *background = nullptr;
	SDL_Texture *musicMenu = nullptr;
	Animation selectedOptionAnim;
	Animation pressStartAnim;
	int optionSelectedIndex;
	int optionsYCoords[6] = {112,166,219,273};

	std::string musicPaths[4] = {"GameMusic/2OutrideaCrisis.ogg","GameMusic/3Sprinter.ogg","GameMusic/4WinningRun.ogg","GameMusic/5HardRoad.ogg"};
	// background color in hex { E0 , A0 , 00 }

public:
	ModuleSceneMusicSelection(bool active);
	~ModuleSceneMusicSelection();
	bool Start();
	update_status Update();
	bool CleanUp();
};


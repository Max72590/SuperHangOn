#pragma once
#include "Module.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include <string>

enum guiMode { GAME_MODE, SCORES_MODE, GAME_OVER_MODE};

class ModuleGUI :
	public Module
{
public:
	ModuleGUI(bool active);
	~ModuleGUI();
	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();
	void setMapSpecificValues(std::string mapName);
	void switchGUImodeToScore(guiMode newMode);
	void updateGUIValues(int time, int score, int speed);

public:
	// Sprites & Textures
	SDL_Texture *sprites;
	SDL_Rect *top;
	SDL_Rect *topCounter ;
	SDL_Rect *time;
	SDL_Rect *timeCounter;
	SDL_Rect *score;
	SDL_Rect *scoreCounter;
	SDL_Rect *speed;
	SDL_Rect *speedCounter;
	SDL_Rect *stageProgression;
	SDL_Rect *course;
	SDL_Rect *courseName;
	SDL_Rect *stage;
	SDL_Rect *stageNumber;
	// Map variables
	int current_stage;
	int elapsed_time = 0;
	int playerScore = 0;
	int playerSpeed = 0;
	int stageNum;
	guiMode actualMode;
};


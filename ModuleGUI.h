#pragma once
#include "Module.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include <string>

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
	void switchGUImodeToScore(bool newMode);
	void updateGUIValues(int time, int score, int speed);

public:
	// Sprites & Textures
	SDL_Texture *sprites;
	SDL_Rect *top = new SDL_Rect({ 32,436,63,27 });
	SDL_Rect *topCounter ;
	SDL_Rect *time = new SDL_Rect({ 96,436,77,27 });
	SDL_Rect *timeCounter;
	SDL_Rect *score = new SDL_Rect({ 176,436,99,27 });
	SDL_Rect *scoreCounter;
	SDL_Rect *speed = new SDL_Rect({ 274,440,83,19 });
	SDL_Rect *speedCounter;
	SDL_Rect *stageProgression;
	SDL_Rect *course = new SDL_Rect({ 408,420,99,19 });
	SDL_Rect *courseName;
	SDL_Rect *stage = new SDL_Rect({ 274,420,83,19 });
	SDL_Rect *stageNumber;
	// Map variables
	int current_stage;
	int elapsed_time = 0;
	int playerScore = 0;
	int playerSpeed = 0;
	int stageNum;
	bool showScores;
};


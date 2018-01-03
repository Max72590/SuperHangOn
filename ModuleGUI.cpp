#include "ModuleGUI.h"
#include "ModuleRender.h"
#include "Application.h"
using namespace std;

ModuleGUI::ModuleGUI(bool active) : Module(active)
{
}


ModuleGUI::~ModuleGUI()
{
}


bool ModuleGUI::Start() {
	sprites = App->textures->Load("rtype/miscellaneous.png");
	// Load default map values
	setMapSpecificValues("asia");
	top = new SDL_Rect({ 32,436,63,27 });
	time = new SDL_Rect({ 96,436,77,27 });
	score = new SDL_Rect({ 176,436,99,27 });
	speed = new SDL_Rect({ 274,440,83,19 });
	course = new SDL_Rect({ 408,420,99,19 });
	stage = new SDL_Rect({ 274,420,83,19 });

	actualMode = GAME_MODE;
	return true;
}

update_status ModuleGUI::Update(float deltaTime) {
	switch (actualMode) {
		case GAME_MODE:
				App->renderer->Blit(sprites, 50, 15, top);
				App->renderer->Blit(sprites, 270, 15, time);
				App->renderer->Blit(sprites, 380, 15, score);
				App->renderer->Blit(sprites, 50, 50, course);
				App->renderer->Blit(sprites, 160, 50, courseName);
				App->renderer->Blit(sprites, 50, 70, stage);
				App->renderer->Blit(sprites, 10, 90, stageProgression);
				App->renderer->Blit(sprites, 400, 50, speed);
				App->fonts->drawMessage(COLOR_RED, to_string(playerScore), 100, 20); //
				App->fonts->drawMessage(COLOR_GREEN, to_string(playerScore), 465, 20);
				App->fonts->drawMessage(COLOR_WHITE, to_string(playerSpeed), 475, 50);
				App->fonts->drawMessage(COLOR_WHITE, std::to_string(stageNum), 145, 70);
				App->fonts->drawMessage(TIME_FONT, to_string(elapsed_time), 270, 50);
				break;
			case SCORES_MODE:
				App->fonts->drawMessage(WHITE_LETTERS, "course", 150, 100);
				App->fonts->drawMessage(WHITE_LETTERS, "clear", 250, 100);
				App->fonts->drawMessage(WHITE_LETTERS, "final", 150, 150);
				App->fonts->drawMessage(WHITE_LETTERS, "score", 250, 150);
				App->fonts->drawMessage(TIME_FONT, to_string(playerScore), 200, 200);
				break;
			case GAME_OVER_MODE:
				App->fonts->drawMessage(WHITE_LETTERS, "game", 150, 100);
				App->fonts->drawMessage(WHITE_LETTERS, "over", 250, 100);
				App->fonts->drawMessage(WHITE_LETTERS, "final", 150, 150);
				App->fonts->drawMessage(WHITE_LETTERS, "score", 250, 150);
				App->fonts->drawMessage(TIME_FONT, to_string(playerScore), 200, 200);
				break;
	}
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {

	delete top;
	delete time;
	delete score;
	delete course;
	delete courseName;
	delete stage;
	delete stageProgression;
	delete speed;
	top = nullptr;
	time = nullptr;
	score = nullptr;
	course = nullptr;
	courseName = nullptr;
	stage = nullptr;
	stageProgression = nullptr;
	speed = nullptr;
	return true;
}

void ModuleGUI::setMapSpecificValues(string mapName) {
	int courseX, coordY, courseWidth;
	if (mapName == "africa") {
		courseX = 264;
		coordY = 466;
		courseWidth = 99;
	}
	else if (mapName ==  "asia") {
		courseX = 520;
		coordY = 506;
		courseWidth = 67;
	}
	else if (mapName == "america") {
		courseX = 392;
		coordY = 486;
		courseWidth = 115;
	}
	else if (mapName == "europe") {
		courseX = 648;
		coordY = 526;		
		courseWidth = 99;
	}
	stageNum = 0;
	courseName = new SDL_Rect({courseX, coordY,courseWidth, 19});
	stageProgression = new SDL_Rect({32,coordY,(courseX-5)-32,19});
}

void ModuleGUI::switchGUImodeToScore(guiMode newMode) {
	actualMode = newMode;
}

void ModuleGUI::updateGUIValues(int time, int score, int speed) {
	elapsed_time = time;
	playerScore = score;
	playerSpeed = speed;
}
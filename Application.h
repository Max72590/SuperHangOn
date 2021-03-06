#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include <time.h>


class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;

// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
class ModuleSceneMapSelection;
class ModuleSceneMusicSelection;
class ModuleRoad;
class ModuleAsiaStage;
class ModuleEnemy;
class ModuleGUI;
class ModuleFonts;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleSceneMusicSelection* music_selec;
	ModuleSceneMapSelection* map_selec;
	ModuleEnemy* enemies;
	ModuleAsiaStage *asiaStage;
	ModuleFonts* fonts;
	ModuleGUI* gui;

private:
	clock_t gameClock;
	float deltaTime;
	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__
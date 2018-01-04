#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleEnemy.h"
#include "ModuleRoad.h"
#include "ModuleAsiaStage.h"
#include "ModuleFonts.h"
#include "ModuleGUI.h"
#include "ModuleSceneMapSelection.h"
#include "ModuleSceneMusicSelection.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	// Game Modules
	modules.push_back(scene_intro = new ModuleSceneIntro(false));
	modules.push_back(map_selec = new ModuleSceneMapSelection(false));
	modules.push_back(music_selec = new ModuleSceneMusicSelection(false));
	modules.push_back(enemies = new ModuleEnemy(false));
	modules.push_back(asiaStage = new ModuleAsiaStage(false));
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(fonts = new ModuleFonts(true));
	modules.push_back(gui = new ModuleGUI(false));

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(fade = new ModuleFadeToBlack());

	
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	//fade->FadeToBlack(scene_intro, nullptr, 3.0f);
	fade->FadeToBlack(asiaStage, nullptr, 3.0f);
	gameClock = clock();
	return ret;
}

update_status Application::Update()
{
	clock_t update = clock();
	deltaTime += (float)(update - gameClock) / CLOCKS_PER_SEC;
	update_status ret = UPDATE_CONTINUE;
	if (deltaTime > STEP ) {
		gameClock = update;
		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			if ((*it)->IsEnabled() == true)
				ret = (*it)->PreUpdate(deltaTime);

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			if ((*it)->IsEnabled() == true)
				ret = (*it)->Update(deltaTime);

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			if ((*it)->IsEnabled() == true)
				ret = (*it)->PostUpdate(deltaTime);
		deltaTime = 0;
	}
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();
	return ret;
}


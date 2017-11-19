#ifndef __MODULESCENEMAPSELECTION_H__
#define __MODULESCENEMAPSELECTION_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneMapSelection :
	public Module
{

public: 
	SDL_Texture* background = nullptr;
	SDL_Texture* mapPieces = nullptr;
	Animation africaSprite;
	Animation americaSprite;
	Animation europeSprite;
	Animation asiaSprite;
	Animation *currentAnimation = nullptr;
	SDL_Rect* beginnerSprite = {};
	SDL_Rect* junioSprite = {};
	SDL_Rect* seniorSprite = {};
	SDL_Rect* expertSprite = {};
	int mapRegionId = 0;
	int xRegion, yRegion;

public:
	ModuleSceneMapSelection(bool active);
	~ModuleSceneMapSelection();
	bool Start();
	update_status Update();
	bool CleanUp();

};
#endif // __MODULESCENEMAPSELECTION_H__

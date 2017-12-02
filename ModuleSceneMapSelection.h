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
	Animation pressStartTitle;
	Animation africaSprite;
	Animation americaSprite;
	Animation europeSprite;
	Animation asiaSprite;
	Animation *currentAnimation = nullptr;
	SDL_Rect* beginnerSprite = new SDL_Rect({ 283,5 ,129,33 });
	SDL_Rect* juniorSprite = new SDL_Rect({ 283, 41 ,143,33 });
	SDL_Rect* seniorSprite = new SDL_Rect({ 429, 5 ,143,33 });
	SDL_Rect* expertSprite = new SDL_Rect({ 429, 41 ,143,33 });
	SDL_Rect* mapSelectionTitle = new SDL_Rect({5,5,273,17});
	int mapRegionId = 0;
	int prevMapRegionId = 0;
	int xRegion, yRegion;
	int widthPadding = 40;
	int heightPadding = 75;
	Animation *animArray[4] = {&africaSprite, &asiaSprite, &americaSprite, &europeSprite};
	SDL_Rect* classSprites[4] = { beginnerSprite ,juniorSprite,seniorSprite,expertSprite };
	int xyRegionCoords[8] = {243,145,315,17,19,49,235,49};
	int xySpriteCoords[8] = {235,177,365,49,61,113,237,81};
	int fxLoadId;
public:
	ModuleSceneMapSelection(bool active);
	~ModuleSceneMapSelection();
	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();

};
#endif // __MODULESCENEMAPSELECTION_H__

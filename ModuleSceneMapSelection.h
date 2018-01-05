#ifndef __MODULESCENEMAPSELECTION_H__
#define __MODULESCENEMAPSELECTION_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneMapSelection :
	public Module
{

public: 
	SDL_Texture* mapPieces = nullptr;
	Animation pressStartTitle;
	Animation africaSprite;
	Animation americaSprite;
	Animation europeSprite;
	Animation asiaSprite;
	Animation *currentAnimation = nullptr;
	SDL_Rect* beginnerSprite;
	SDL_Rect* juniorSprite;
	SDL_Rect* seniorSprite;
	SDL_Rect* expertSprite;
	SDL_Rect* mapSelectionTitle;
	int mapRegionId = 0;
	int prevMapRegionId = 0;
	int xRegion, yRegion;
	int widthPadding = 40;
	int heightPadding = 75;
	std::vector<Animation*> animVector; 
	std::vector<SDL_Rect*> classSprites;
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

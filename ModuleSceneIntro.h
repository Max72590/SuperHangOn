#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool active = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();


public:
	bool hasSegaLogoFinished,firstOptionIsSelected,newGameSelected;
	SDL_Texture* background = nullptr;
	SDL_Texture* segaLogoSprites = nullptr;
	SDL_Texture* introSegaLogoSprites = nullptr;
	SDL_Texture* superHangOnSprites = nullptr;
	SDL_Texture* menuOptions = nullptr;

	SDL_Rect* logoRect = {};
	SDL_Rect* menuOptionsRect = {};
	SDL_Rect* menuOptionsGameType = {};
	SDL_Rect* menuCursor = {};

	uint fx = 0;
	Animation introSegaLogo;
	Animation introSuperHangOnLogo;
	Animation segaLogo;
	Animation superHangOnLogo;
};

#endif // __MODULESCENEINTRO_H__
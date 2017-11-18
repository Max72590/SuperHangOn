#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMapSelection.h"



ModuleSceneMapSelection::ModuleSceneMapSelection()
{
}


ModuleSceneMapSelection::~ModuleSceneMapSelection()
{
}

bool ModuleSceneMapSelection::Start() {
	LOG("Loading space intro");
	background = App->textures->Load("rtype/mapSelectionBackground.png");
	mapPieces = App->textures->Load("rtype/miscellaneous.png");

	africaSprite.frames.push_back({21,131,272,256}); 
	africaSprite.frames.push_back({ 1100,10,272,256 });
	africaSprite.speed = 0.5;
	africaSprite.loop = true;
	europeSprite.frames.push_back({ 697,7,113,97 });
	europeSprite.frames.push_back({ 1100,10,113,97 });
	europeSprite.speed = 0.5;
	europeSprite.loop = false;
	americaSprite.frames.push_back({ 566,122,113,97 });
	americaSprite.frames.push_back({ 1100,10,113,97 });
	americaSprite.speed = 0.5;
	americaSprite.loop = false;
	asiaSprite.frames.push_back({ 787,107,882,357 });
	asiaSprite.frames.push_back({ 1100,10,882,357 });
	asiaSprite.speed = 0.5;
	asiaSprite.loop = false;

	beginnerSprite = new SDL_Rect(); // {283,5 ,129,33};
	beginnerSprite->x = 283;
	beginnerSprite->y = 5;
	beginnerSprite->w = 129;
	beginnerSprite->h = 33;
	junioSprite = new SDL_Rect(); //{283, 41 ,143,33};
	junioSprite->x = 283;
	junioSprite->y = 41;
	junioSprite->w = 143;
	junioSprite->h = 33;
	seniorSprite = new SDL_Rect(); //{429, 5 ,143,33};
	seniorSprite->x = 429;
	seniorSprite->y = 5;
	seniorSprite->w = 143;
	seniorSprite->h = 33;
	expertSprite = new SDL_Rect(); //{429, 41 ,143,33};
	expertSprite->x = 429;
	expertSprite->y = 41;
	expertSprite->w = 143;
	expertSprite->h = 33;
	return true;
}

update_status ModuleSceneMapSelection::Update() {
	switch (mapRegionId)
	{
	case 0:
		currentAnimation = &africaSprite;
		break;
	case 1:
		currentAnimation = &asiaSprite;
		break;
	case 2:
		currentAnimation = &americaSprite;
		break;
	case 3:
		currentAnimation = &europeSprite;
		break;
	}
	App->renderer->Blit(mapPieces, xRegion, yRegion, &currentAnimation->GetCurrentFrame());
	return UPDATE_CONTINUE;
}

bool ModuleSceneMapSelection::CleanUp() {
	return true;
}
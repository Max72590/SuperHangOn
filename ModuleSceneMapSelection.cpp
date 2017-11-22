#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMapSelection.h"



ModuleSceneMapSelection::ModuleSceneMapSelection(bool active): Module(active)
{
}


ModuleSceneMapSelection::~ModuleSceneMapSelection()
{
}

bool ModuleSceneMapSelection::Start() {
	LOG("Loading space intro");
	background = App->textures->Load("rtype/mapSelectionBackground.png");
	mapPieces = App->textures->Load("rtype/miscellaneous.png");
	pressStartTitle.frames.push_back({ 5,27,273,17 });
	pressStartTitle.frames.push_back({ 0,1250,273,17 });
	pressStartTitle.speed = 0.05;
	pressStartTitle.loop = true;

	africaSprite.frames.push_back({587,4,103,112}); 
	africaSprite.frames.push_back({ 0,1250,103,112 });
	africaSprite.speed = 0.1;
	africaSprite.loop = true;
	europeSprite.frames.push_back({ 697,7,113,97 });
	europeSprite.frames.push_back({ 0,1250,113,97 });
	europeSprite.speed = 0.1;
	europeSprite.loop = false;
	americaSprite.frames.push_back({ 566,122,193,257 });
	americaSprite.frames.push_back({ 0,1250,193,257 });
	americaSprite.speed = 0.1;
	americaSprite.loop = false;
	asiaSprite.frames.push_back({ 787,107,233,272 });
	asiaSprite.frames.push_back({ 0,1250,233,272 });
	asiaSprite.speed = 0.1;
	asiaSprite.loop = false;

	currentAnimation = &africaSprite;
	fxLoadId = App->audio->LoadFx("GameFX/starting.wav");
	App->audio->PlayMusic("GameMusic/1SelectYourClass.ogg");
	return true;
}

update_status ModuleSceneMapSelection::Update() {
	App->renderer->Blit(background, 0, 0, &(SDL_Rect({ 0,0,640,480 })));
	if (prevMapRegionId != mapRegionId) {
		currentAnimation->loop = false;
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
		currentAnimation->loop = true;
		prevMapRegionId = mapRegionId;	
	}
	for (int i = 0; i < 4; ++i) {
		if (i != mapRegionId) App->renderer->Blit(mapPieces, xyRegionCoords[i * 2] + widthPadding, xyRegionCoords[i * 2 + 1] + heightPadding, &animArray[i]->frames[0]);
		else App->renderer->Blit(mapPieces, xyRegionCoords[mapRegionId * 2] + widthPadding, xyRegionCoords[mapRegionId * 2 + 1] + heightPadding, &currentAnimation->GetCurrentFrame());
		App->renderer->Blit(mapPieces, xySpriteCoords[i * 2] + widthPadding, xySpriteCoords[i * 2 + 1] + heightPadding, classSprites[i]);
	}
	App->renderer->Blit(mapPieces, 184 , 23 , mapSelectionTitle);
	App->renderer->Blit(mapPieces, 184 , 373, &pressStartTitle.GetCurrentFrame());
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (mapRegionId <= 0) mapRegionId = 3;
		else --mapRegionId;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (mapRegionId >= 3) mapRegionId = 0;
		else ++mapRegionId;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->music_selec, this);
		App->audio->PlayFx(fxLoadId);
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneMapSelection::CleanUp() {
	/*delete &africaSprite;
	delete &asiaSprite;
	delete &americaSprite;
	delete &europeSprite;
	delete beginnerSprite;
	delete juniorSprite;
	delete seniorSprite;
	delete expertSprite;
	delete background;
	delete mapPieces;
	delete[] animArray;
	delete[] classSprites;*/
	return true;
}
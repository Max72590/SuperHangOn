#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneIntro::ModuleSceneIntro(bool active) : Module(active)
{
	int spriteY = 39;
	int offset = 185;
	for (int i = 0; i < 6; ++i) {
		introSuperHangOnLogo.frames.push_back({ 57,spriteY,532,144 });
		introSuperHangOnLogo.frames.push_back({ 697,spriteY,532,144 });
		introSuperHangOnLogo.frames.push_back({ 1337,spriteY,532,144 });
		spriteY += offset;
	}
	introSuperHangOnLogo.loop = false;
	introSuperHangOnLogo.speed = 0.2f;

	offset = 185;
	spriteY = 1519;
	for (int i = 0; i < 8; ++i) {
		superHangOnLogo.frames.push_back({ 57,spriteY,532,144 });
		superHangOnLogo.frames.push_back({ 697,spriteY,532,144 });
		superHangOnLogo.frames.push_back({ 1337,spriteY,532,144 });
		spriteY += offset;
	}
	superHangOnLogo.loop = true;
	superHangOnLogo.speed = 0.3f;

	spriteY = 135;
	offset = 65;
	for (int i = 0; i < 12; ++i) {
		introSegaLogo.frames.push_back({ 217,spriteY,188,61 });
		introSegaLogo.frames.push_back({ 857,spriteY,188,61 });
		introSegaLogo.frames.push_back({ 1497,spriteY,188,61 });
		spriteY += offset;
	}
	introSegaLogo.loop = false;
	introSegaLogo.speed = 0.2f;

	spriteY = 1;
	offset = 53;
	for (int i = 0; i < 5; ++i) {
		segaLogo.frames.push_back({ 241,spriteY,148,50 });
		segaLogo.frames.push_back({ 241,spriteY,148,50 });
		segaLogo.frames.push_back({ 241,spriteY,148,50 });
		spriteY += offset;
	}
	segaLogo.loop = true;
	segaLogo.speed = 0.2f;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading space intro");
	introSegaLogoSprites = App->textures->Load("sprites/segaLogo.bmp");
	segaLogoSprites = App->textures->Load("sprites/segaLogoMenuOne.bmp");
	superHangOnSprites = App->textures->Load("sprites/superHangOnLogo.bmp");
	menuOptions = App->textures->Load("sprites/menuOneOptions.bmp");
	fx = App->audio->LoadFx("GameFX/starting.wav");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	hasSegaLogoFinished = false;
	introSuperHangOnLogo.Reset();
	superHangOnLogo.Reset();
	introSegaLogo.Reset();
	segaLogo.Reset();
	menuOptionsRect = new SDL_Rect();
	menuOptionsRect->x = 257;
	menuOptionsRect->y = 1;
	menuOptionsRect->w = 124;
	menuOptionsRect->h = 44;
	
	menuCursor = new SDL_Rect();
	menuCursor->x = 3;
	menuCursor->y = 69;
	menuCursor->w = 9;
	menuCursor->h = 13;

	menuOptionsGameType = new SDL_Rect();
	menuOptionsGameType->x = 209;
	menuOptionsGameType->y = 85;
	menuOptionsGameType->w = 204;
	menuOptionsGameType->h = 44;

	firstOptionIsSelected = true;
	newGameSelected = false;
	return true;
}
//384
// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading space scene");

	delete logoRect;
	delete menuOptionsRect;
	delete menuOptionsGameType;
	delete menuCursor;

	App->textures->Unload(introSegaLogoSprites);
	App->textures->Unload(segaLogoSprites);
	App->textures->Unload(superHangOnSprites);
	App->textures->Unload(menuOptions);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float deltaTime)
{

	App->renderer->DrawQuad(SDL_Rect({ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }), 0, 0, 0, 255);
	if (introSegaLogo.Finished()) {
		if (introSuperHangOnLogo.Finished()) {
			if (!newGameSelected) App->renderer->Blit(menuOptions, 250, 280, menuOptionsRect);
			else  App->renderer->Blit(menuOptions, 250, 280, menuOptionsGameType);
			App->renderer->Blit(segaLogoSprites, 246, 400, &segaLogo.GetCurrentFrame());
			App->renderer->Blit(superHangOnSprites, 54, 30, &superHangOnLogo.GetCurrentFrame());
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				if (!firstOptionIsSelected) firstOptionIsSelected = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				if (firstOptionIsSelected) firstOptionIsSelected = false;
			}
			if (firstOptionIsSelected)	App->renderer->Blit(menuOptions, 230, 280, menuCursor);
			else App->renderer->Blit(menuOptions, 230, 312, menuCursor);

		}
		else {
			App->renderer->Blit(superHangOnSprites, 54, 30, &introSuperHangOnLogo.GetCurrentFrame());
		}
	}
	else {
		App->renderer->Blit(introSegaLogoSprites, 226, 210, &introSegaLogo.GetCurrentFrame());
	}
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		if (firstOptionIsSelected && !newGameSelected)	newGameSelected = true;
		else App->fade->FadeToBlack((Module*)App->map_selec, this);		
		App->audio->PlayFx(fx);
	}

	return UPDATE_CONTINUE;
}
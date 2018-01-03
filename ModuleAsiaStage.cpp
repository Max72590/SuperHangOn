#include "ModuleAsiaStage.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleGUI.h"
#include "ModuleFadeToBlack.h"




ModuleAsiaStage::ModuleAsiaStage(bool active) : ModuleRoad(active)
{
	semaphore.frames.push_back({ 339, 421, 31, 73 });
	semaphore.frames.push_back({ 370, 421, 31, 73 });
	semaphore.frames.push_back({ 401, 421, 31, 73 });
	semaphore.frames.push_back({ 432, 421, 31, 73 });
	semaphore.loop = false;
	semaphore.speed = 0.016f;
}


ModuleAsiaStage::~ModuleAsiaStage()
{
}

bool ModuleAsiaStage::Start() {
	// Enabling modules
	App->fonts->Enable();
	App->gui->Enable();
	App->gui->switchGUImodeToScore(GAME_MODE);
	App->enemies->Enable();
	App->player->Enable();
	App->collision->Enable();
	//Loading assets
	background = App->textures->Load("rtype/backgrounds.png");
	roadDecorations = App->textures->Load("rtype/sprites.png");
	roadSigns = App->textures->Load("rtype/stuff.png");
	// Initiating stage
	camZPosition = 0;
	endSegmentIndex = 100;
	stageColorChangeIndexes.push_back(300);
	stageColorChangeIndexes.push_back(1500);
	stageColorChangeIndexes.push_back(2000);
	semaphore.Reset();
	roadLength = roadPoints.size();
	for (int i = 0; i < 5000; ++i) {
		roadPoint *rp = new roadPoint();
		rp->worldZ = i*segmentLength;
		// Curves
		if (i > 300 && i < 500) rp->curvefactor = 2;
		else if (i > 2000 && i < 2200) rp->curvefactor = -2;
		else if (i > 2400 && i < 2600) rp->curvefactor = 3;
		// Hills
		if (i > 0) {
			if (i >= 720 && i <= 970) {
				if (i == 721) {
					float cos1 = cos(720.0f / 30.0f) * 1000.0f;
					float cos2 = cos(719.0f / 30.0f) * 1000.0f;
					smoothInOut(719, 720, abs( abs(cos1) - abs(cos2) ));
				}
				rp->worldY = cos(i / 30.0f) * 1000.0f;
			}
			else rp->worldY = (*roadPoints[i - 1]).worldY;
		}
		else rp->worldY = 0;
		roadPoints.push_back(rp);
	}
	roadLength = roadPoints.size();
	camDepth = 0.84f; // Horizon looks better with this value rather than this one-> camDepth = 0.58 
	sky.push_back (new SDL_Rect({ 8,67,512 ,36 }));
	sky.push_back(new SDL_Rect({ 871,141,512 ,36 }));
	sky.push_back(new SDL_Rect({ 871,251,512 ,36 }));
	foreground.push_back(new SDL_Rect({ 534,63,319,12 }));
	foreground.push_back(new SDL_Rect({ 530,170,319,12 }));
	foreground.push_back(new SDL_Rect({ 530,210,319,12 }));


	(*roadPoints[endSegmentIndex]).prop->spriteID = 3;
	(*roadPoints[endSegmentIndex]).prop->spriteXCoord = -1.5f + (*roadPoints[25]).curvefactor;
	(*roadPoints[endSegmentIndex]).prop->tex = roadSigns;
	(*roadPoints[endSegmentIndex]).prop->scalefactor = 4.0f;
	(*roadPoints[20]).prop->spriteID = -2;
	(*roadPoints[20]).prop->spriteXCoord = -10.0f + (*roadPoints[20]).curvefactor;
	(*roadPoints[20]).prop->tex = roadSigns;
	(*roadPoints[20]).prop->scalefactor = 2.0f;
	(*roadPoints[25]).prop->spriteID = 1;
	(*roadPoints[25]).prop->spriteXCoord = -1.5f + (*roadPoints[25]).curvefactor;
	(*roadPoints[25]).prop->tex = roadSigns;
	(*roadPoints[25]).prop->scalefactor = 4.0f;
	(*roadPoints[500]).prop->spriteID = 2;
	(*roadPoints[500]).prop->spriteXCoord = -1.5f + (*roadPoints[25]).curvefactor;
	(*roadPoints[500]).prop->tex = roadSigns;
	(*roadPoints[500]).prop->scalefactor = 4.0f;
	(*roadPoints[2000]).prop->spriteID = 2;
	(*roadPoints[2000]).prop->spriteXCoord = -1.5f + (*roadPoints[25]).curvefactor;
	(*roadPoints[2000]).prop->tex = roadSigns;
	(*roadPoints[2000]).prop->scalefactor = 4.0f;
	(*roadPoints[3500]).prop->spriteID = 2;
	(*roadPoints[3500]).prop->spriteXCoord = -1.5f + (*roadPoints[25]).curvefactor;
	(*roadPoints[3500]).prop->tex = roadSigns;
	(*roadPoints[3500]).prop->scalefactor = 4.0f;
	SDL_Rect *tree = new SDL_Rect({ 1204,4,283,296 });
	SDL_Rect *startSign = new SDL_Rect({ 332, 1, 315, 104 });
	SDL_Rect *checkSign = new SDL_Rect({ 332, 107, 315, 104 });
	SDL_Rect *goalSign = new SDL_Rect({ 332, 213, 315, 104 });
	SDL_Rect *leftSign = new SDL_Rect({ 5, 3, 47, 43 });
	SDL_Rect *rightSign = new SDL_Rect({ 60, 3, 47, 43 });
	sprites.push_back(tree);
	sprites.push_back(startSign);
	sprites.push_back(checkSign);
	sprites.push_back(goalSign);
	sprites.push_back(leftSign);
	sprites.push_back(rightSign);
	for (int j = 50; j < 300; ++j) {
		if ((j % 20) == 0) {
			(*roadPoints[j]).prop->spriteID = 0;
			(*roadPoints[j]).prop->spriteXCoord = -2.0f + (*roadPoints[j]).curvefactor;
			(*roadPoints[j]).prop->collider = App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);
			(*roadPoints[j]).prop->tex = roadDecorations;
		}
	}
	for (int j = 720; j < 970; ++j) {
		if ((j % 20) == 0) {
			(*roadPoints[j]).prop->spriteID = 4;
			(*roadPoints[j]).prop->spriteXCoord = 5.0f + (*roadPoints[j]).curvefactor;
			(*roadPoints[j]).prop->scalefactor = 2.0f;
			(*roadPoints[j]).prop->collider = App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);
			(*roadPoints[j]).prop->tex = roadSigns;
		}
	}
	for (int j = 2000; j < 2200; ++j) {
		if ((j % 20) == 0) {
			(*roadPoints[j]).prop->spriteID = 4;
			(*roadPoints[j]).prop->spriteXCoord = 10.0f + (*roadPoints[j]).curvefactor;
			(*roadPoints[j]).prop->scalefactor = 2.0f;
			(*roadPoints[j]).prop->collider = App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);		
			(*roadPoints[j]).prop->tex = roadSigns;
		}
	}
	for (int j = 2400; j < 2600; ++j) {
		if ((j % 20) == 0) {
			(*roadPoints[j]).prop->spriteID = 5;
			(*roadPoints[j]).prop->spriteXCoord = -10.0f + (*roadPoints[j]).curvefactor;
			(*roadPoints[j]).prop->scalefactor = 2.0f;
			(*roadPoints[j]).prop->collider = App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);			
			(*roadPoints[j]).prop->tex = roadSigns;
		}
	}
	App->enemies->addEnemy(*(App->enemies->greenEnemy), -roadWidth / 2, 20);
	App->enemies->addEnemy(*(App->enemies->greenEnemy), roadWidth / 2, 20);
	App->enemies->addEnemy(*(App->enemies->yellowEnemy), 0, 40);
	for (int i = 0; i < (int)App->enemies->enemies.size(); ++i) {
		App->enemies->enemies[i]->collider = App->collision->AddCollider(App->enemies->enemies[i]->current_animation->GetCurrentFrame(), ENEMY);
	}
	backgroundPosX = -128;
	runTimer = false;
	runGameOverTimer = false;
	timerAcum = 0.0f;
	raceSeconds = 60;
	gameOverCountdown = 10;
	colorIndex = 0;
	stageColors.push_back(colors());
	stageColors[0].sky = SDL_Color({128,224,224,255});
	stageColors[0].roadDark = SDL_Color({ 62,62,62,255 });
	stageColors[0].roadLight = SDL_Color({ 100,100,100,255 });
	stageColors[0].offroadDark = SDL_Color({ 0,150,0,255 });
	stageColors[0].offroadLight = SDL_Color({ 0,200,0,255 });
	stageColors.push_back(colors());
	stageColors[1].sky = SDL_Color({ 128,32,224,255 });
	stageColors[1].roadDark = SDL_Color({ 96,64,64,255 });
	stageColors[1].roadLight = SDL_Color({ 128,128,128,255 });
	stageColors[1].offroadDark = SDL_Color({ 224,160,32,255 });
	stageColors[1].offroadLight = SDL_Color({ 224,192,64,255 });
	stageColors.push_back(colors());
	stageColors[2].sky = SDL_Color({ 224,96,0,255 });
	stageColors[2].roadDark = SDL_Color({ 128,96,0,255 });
	stageColors[2].roadLight = SDL_Color({ 160,96,64,255 });
	stageColors[2].offroadDark = SDL_Color({ 160,128,32,255 });
	stageColors[2].offroadLight = SDL_Color({ 192,160,64,255 });
	return true;
}



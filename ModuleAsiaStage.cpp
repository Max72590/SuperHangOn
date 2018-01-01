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
}


ModuleAsiaStage::~ModuleAsiaStage()
{
}

bool ModuleAsiaStage::Start() {
	camZPosition = 0;
	roadLength = roadPoints.size();
	for (float i = 0; i < 5000; ++i) {
		roadPoint *rp = new roadPoint();
		rp->worldZ = i*segmentLength;
		// Curves
		if (i > 300 && i < 500) rp->curvefactor = 2;
		else if (i > 2000 && i < 2200) rp->curvefactor = -2;
		else if (i > 2400 && i < 2600) rp->curvefactor = 3;
		// Hills
		if (i > 0) {
			if (i >= 720 && i <= 970) {
				if (i == 721) smoothInOut(719, 720, abs(cosinus(720) - cosinus(719)));
				rp->worldY = cosinus(i);
			}

			else rp->worldY = (*roadPoints[i - 1]).worldY;
		}
		else rp->worldY = 0;
		roadPoints.push_back(rp);

	}
	roadLength = roadPoints.size();
	camDepth = 0.84f; // Horizon looks better with this value
					  //camDepth = 0.58 
	sky = new SDL_Rect({ 8,67,512 ,36 });
	foreground = new SDL_Rect({ 534,63,319,12 });
	background = App->textures->Load("rtype/backgrounds.png");
	roadDecorations = App->textures->Load("rtype/sprites.png");
	roadSigns = App->textures->Load("rtype/stuff.png");
	semaphore.frames.push_back({ 339, 421, 31, 73 });
	semaphore.frames.push_back({ 370, 421, 31, 73 });
	semaphore.frames.push_back({ 401, 421, 31, 73 });
	semaphore.frames.push_back({ 432, 421, 31, 73 });
	semaphore.loop = false;
	semaphore.speed = 0.016f;
	endSegmentIndex = 4000;
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
	runGameOverTimer = false;
	timerAcum = 0;
	raceSeconds = 60;
	gameOverCountdown = 10;
	//App->player->Enable(); // deret dis
	return true;
}



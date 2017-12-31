#include "ModuleRoad.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleGUI.h"
#include "ModuleFadeToBlack.h"

using namespace std;

ModuleRoad::ModuleRoad(bool active): Module(active)
{
}


ModuleRoad::~ModuleRoad()
{
}


bool ModuleRoad::Start() {
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
				if (i == 721) smoothInOut(719, 720, abs( cosinus(720) - cosinus(719)));
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
	sky = new SDL_Rect({8,67,512 ,36});
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
	SDL_Rect *tree = new SDL_Rect({1204,4,283,296});
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
			(*roadPoints[j]).prop->collider= App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);
			(*roadPoints[j]).prop->tex = roadDecorations ;
		}
	}
	for (int j =720; j < 970; ++j) {
		if ((j % 20) == 0 ) {
			(*roadPoints[j]).prop->spriteID = 4;
			(*roadPoints[j]).prop->spriteXCoord = -4.0f + (*roadPoints[j]).curvefactor;
			(*roadPoints[j]).prop->collider = App->collision->AddCollider(*sprites[(*roadPoints[j]).prop->spriteID], WALL);
			(*roadPoints[j]).prop->tex = roadSigns;
		}
	}
	App->enemies->addEnemy(*(App->enemies->greenEnemy), -roadWidth/2, 20);
	for (int i = 0; i < (int)App->enemies->enemies.size(); ++i) {
		App->enemies->enemies[i]->collider = App->collision->AddCollider(App->enemies->enemies[i]->current_animation->GetCurrentFrame() , ENEMY);
		switch (i) {
			case 0:
				App->enemies->enemies[i]->setSpeed(0.5f);
				break;
			case 1:
				App->enemies->enemies[i]->setSpeed(0.7f);
				break;
			case 2:
				App->enemies->enemies[i]->setSpeed(0.9f);
				break;
			case 3:
				App->enemies->enemies[i]->setSpeed(1.1f);
				break;
			case 4:
				App->enemies->enemies[i]->setSpeed(1.3f);
				break;
		}
	}

	backgroundPosX = -128;
	runTimer = true;
	runGameOverTimer = false;
	timerAcum = 0;
	raceSeconds = 10;
	gameOverCountdown = 10;
	//App->player->Enable(); // deret dis
	return true;
}


update_status ModuleRoad::Update(float deltaTime) {

	App->renderer->DrawQuad(SDL_Rect({0,0,640,480}), 128,224,224,255);
	App->renderer->ScaledBlit(background, (int)backgroundPosX, 204, sky, sky->w * 2, sky->h * 2);
	App->renderer->ScaledBlit(background, (int)foregroundPosX, 252, foreground, foreground->w * 2, foreground->h * 2);
	camZPosition = calculatePosZ(App->player->getSpeed()*deltaTime*40);
	bool actualSeg = (((int)(camZPosition / segmentLength)) >= endSegmentIndex);
	if (semaphore.Finished()) {
		App->enemies->startRace();
		runTimer = true;
	}
	if ( actualSeg && !crossedEndSegment ){
		crossedEndSegment = true;
		setUpEnding(false);
		runGameOverTimer = true;
	}
	paintRoad(deltaTime);
	if (runTimer) {
		timerAcum += deltaTime;
		if (timerAcum > 1.0f) {
			timerAcum = 0.0f;
			if (raceSeconds > 0) --raceSeconds;
			else if (raceSeconds <= 0 && !crossedEndSegment) {
				setUpEnding(true);
				runGameOverTimer = true;
			}
			if (runGameOverTimer) {				
				if (gameOverCountdown > 0) --gameOverCountdown;
				else {
					App->fade->FadeToBlack((Module*)App->scene_intro, this);
					App->stopGameModules();
				}
			}
		}
		App->gui->updateGUIValues(raceSeconds, App->player->getScore(), App->player->getSpeed() );
	}
	return UPDATE_CONTINUE;
}

bool ModuleRoad::CleanUp() {
	App->textures->Unload(background);
	App->textures->Unload(roadDecorations);
	App->player->Disable();
	for (int i = 0; i < (int)enemies.size(); ++i) RELEASE (enemies[i]);
	enemies.clear();
	for (int i = 0; i < (int)sprites.size(); ++i) RELEASE(sprites[i]);
	sprites.clear();
	for (int i = 0; i < (int)roadPoints.size(); ++i) {
		RELEASE(roadPoints[i]->prop);
		RELEASE(roadPoints[i]);
	}
	roadPoints.clear();
	sky = nullptr;
	background = nullptr;
	foreground = nullptr;
	roadDecorations = nullptr;
	delete sky;
	delete background;
	delete foreground;
	delete roadDecorations;
	return true;
}

void ModuleRoad::resetRoad() {
	camZPosition = 0;
	backgroundPosX = -128;
	foregroundPosX = 0;
	offsetX = 0;
	roadX = 0;
	App->enemies->enemies[0]->setPos(-roadWidth/2, 20);
	runTimer = false;
	runGameOverTimer = false;
	timerAcum = 0.0f;
	raceSeconds = 50;
	gameOverCountdown = 10;
}


void ModuleRoad::paintRoad(float deltaTime) {
	while (camZPosition >= roadLength*200) camZPosition -= roadLength*segmentLength;
	while (camZPosition < 0) camZPosition += roadLength*segmentLength;
	float ymax = SCREEN_HEIGHT;
	int initPos = (int)(camZPosition / segmentLength);
	roadPoint* rp = roadPoints[initPos];
	if (rp->prop->spriteID == 2) raceSeconds += 30;
	backgroundPosX += rp->curvefactor / 16;
	foregroundPosX += rp->curvefactor/4;
	camHeight = (int)(1500 + rp->worldY);
	offsetX = 0;
	roadX = 0;
	App->player->offsetX(-(rp->curvefactor)*deltaTime);
	for (int i = initPos; i < initPos + drawDistance; ++i) {
		roadPoint *rpActual = roadPoints[i%roadLength];
		projection(*rpActual, (i>= roadLength) );
		roadX += offsetX;
		offsetX += rpActual->curvefactor;
		rpActual->clipCoord = (float) ymax;
		if ( (rpActual->screenY >= ymax) ) continue;
		ymax = rpActual->screenY;
		(i == 0 ? ++i : i );
		roadPoint *rpPrevious = roadPoints[(i - 1)%roadLength];
		drawTrack( (rpPrevious), (rpActual), ((i/3)%2 == 0 ));		
	}
	drawSprites(initPos);
	roadPoint* nextpoint = roadPoints[(initPos + 1) % roadLength];
	checkCollisions(nextpoint);
}

void ModuleRoad::projection(roadPoint &rp, bool looped) {
	float camX = rp.worldX - (int)(App->player->getValueX() - roadX);
	float camY = rp.worldY - camHeight;
	float camZ = rp.worldZ - (camZPosition - (looped ? roadLength*segmentLength : 0));
	rp.screenScale = camDepth / camZ;
	rp.screenX = ( (1.0f + (rp.screenScale * camX) ) * (SCREEN_WIDTH / 2.0f)) ;
	rp.screenY = ( (1.0f - (rp.screenScale * camY) ) * (SCREEN_HEIGHT/1.8f)); 
	rp.screenW = (rp.screenScale*roadWidth * (SCREEN_WIDTH / 2) );
}

void ModuleRoad::drawTrack(roadPoint const *p1, roadPoint const *p2, bool const isColor1) {
	assert(p1 != nullptr);
	assert(p2 != nullptr);
	Sint16 p1x = (Sint16)p1->screenX;
	Sint16 p1w = (Sint16)p1->screenW;
	Sint16 p2x = (Sint16)p2->screenX;
	Sint16 p2w = (Sint16)p2->screenW;
	Sint16 y1 = (Sint16)p1->screenY;
	Sint16 y2 = (Sint16)p2->screenY;
	Sint16 roadX[4] = {p1x-p1w, p2x-p2w, p2x+p2w, p1x + p1w };
	Sint16 greenX[4] = { 0, 0,SCREEN_WIDTH,SCREEN_WIDTH };
	Sint16 leftRumbleX[4] = {p1x-p1w,p2x-p2w,p2x - p2w -p2w*0.05f,p1x-p1w -p1w*0.05f };
	Sint16 rightRumbleX[4] = {p1x+p1w,p2x+p2w,p2x+p2w+p2w*0.05f,p1x+p1w+p1w*0.05f };
	Sint16 y[4] = { y1,y2,y2,y1 };
	SDL_Color roadColor, offRoadColor, rumbleColor;
	if (isColor1) {
		roadColor = ROAD_LIGHT;
		offRoadColor = GREEN_DARK;
		rumbleColor = WHITE;
	}
	else {
		roadColor = ROAD_DARK;
		offRoadColor = GREEN_LIGHT;
		rumbleColor = RED;
	}
	filledPolygonRGBA(App->renderer->renderer, greenX, y, 4, offRoadColor.r, offRoadColor.g, offRoadColor.b, offRoadColor.a);
	filledPolygonRGBA(App->renderer->renderer, roadX, y, 4, roadColor.r, roadColor.g, roadColor.b, roadColor.a);
	filledPolygonRGBA(App->renderer->renderer, rightRumbleX, y, 4, rumbleColor.r, rumbleColor.g, rumbleColor.b, rumbleColor.a);
	filledPolygonRGBA(App->renderer->renderer, leftRumbleX, y, 4, rumbleColor.r, rumbleColor.g, rumbleColor.b, rumbleColor.a);
}

void ModuleRoad::drawSprites(int initPos) {
	assert(initPos >= 0);
	// Scenery
	for (int j = initPos + (int)drawDistance; j > initPos; --j) {
		roadPoint *roadP = roadPoints[j % roadLength];
		if (roadP->prop->spriteID != -1) {
			SDL_Rect sprite;
			if (roadP->prop->spriteID > -1) sprite = *sprites[roadP->prop->spriteID];
			else if (roadP->prop->spriteID == -2) sprite = semaphore.GetCurrentFrame();
			float spriteX = roadP->screenX + roadP->screenScale * roadP->prop->spriteXCoord * (SCREEN_WIDTH / 2);
			float spriteY = roadP->screenY;
			float spriteW = sprite.w * (roadP->screenW / segmentLength);
			float spriteH = sprite.h * (roadP->screenW / segmentLength);
			spriteX += spriteW * roadP->prop->spriteXCoord;
			spriteY += spriteH *roadP->prop->scalefactor * (-1);
			float clipH = spriteY + spriteH - roadP->clipCoord;
			if (clipH < 0) clipH = 0;
			if (clipH >= spriteH) continue;
			sprite.h = (int)(sprite.h - (sprite.h * clipH / spriteH));
			App->renderer->ScaledBlit(roadP->prop->tex, (int)(spriteX - spriteW / 2), (int)spriteY, &sprite, (int)spriteW*roadP->prop->scalefactor, (int)spriteH*roadP->prop->scalefactor);
			if (roadP->prop->collider != nullptr) {
				roadP->prop->collider->setPos((int)(spriteX - spriteW / 2), (int)spriteY);
				roadP->prop->collider->setWidthHeight((int)spriteW, (int)spriteH);
			}
		}
	}
	// Enemies
	for (int k = 0; k < (int)App->enemies->enemies.size(); ++k) {
		Enemy *e = App->enemies->enemies[k];
		if (e->enemyPosZ > initPos && e->enemyPosZ < initPos+drawDistance && e->enemyEnabled()) {
			roadPoint *roadP = roadPoints[((int) e->getPosZ()) % roadLength];
			SDL_Rect *sprite = App->enemies->enemies[k]->getActualAnimRect();
			float spriteX = roadP->screenX + roadP->screenScale * e->enemyPosX * (SCREEN_WIDTH / 2);
			float spriteY = roadP->screenY + 4;
			float spriteW = sprite->w * (roadP->screenW / 266);
			float spriteH = sprite->h * (roadP->screenW / 266);
			//spriteX += spriteW *  e->enemyPosX;
			spriteY += spriteH * (-1);
			float clipH = spriteY + spriteH - roadP->clipCoord;
			if (clipH < 0) clipH = 0;
			if (clipH >= spriteH) return;
			e->setMoveAnim(roadP->curvefactor);
			App->enemies->drawEnemy(spriteX, spriteY, spriteW, spriteH, e);
			App->enemies->updateEnemyCollider(e, (int)spriteX, (int)spriteY, (int)spriteW, (int)spriteH);
		}
	}
}

void ModuleRoad::smoothInOut(int previouspPos, int actualPos, float amount) {
	float actualHeight = (*roadPoints[actualPos]).worldY;
	float previousHeight = (*roadPoints[previouspPos]).worldY;
	float acumulator = previousHeight + amount;
	int steps = (int)ceil((actualHeight - previousHeight) / amount);
	for (int i = actualPos - steps; i < actualPos; ++i) {
		(*roadPoints[i]).worldY = acumulator;
		if (actualHeight > previousHeight) acumulator += amount;
		else  acumulator -= amount;
	}
}

float ModuleRoad::cosinus(float rads) {
	return cos(rads / 30) * 1000;
}


float ModuleRoad::calculatePosZ(float speed) {
	realPosZ += (int)speed;
	if (realPosZ > 200) {
		int numSteps = (int)(realPosZ / segmentLength);
		if (!runGameOverTimer)App->player->addScore(200*numSteps);
		camZPosition += numSteps * 200;
		realPosZ -= numSteps * 200;
	}
	return camZPosition;
}

void ModuleRoad::checkCollisions(roadPoint *rp) {
	assert(rp != nullptr);
	if (rp->prop->spriteID > -1 && rp->prop->collider != nullptr) {
		if (App->player->collider->checkCollisionCoordX(rp->prop->collider->rect)) {
			App->player->setPlayerState(FALLING);
		}
	}
	for (int k = 0; k < (int)App->enemies->enemies.size(); ++k) {
		Enemy *e = App->enemies->enemies[k];
		if (e->getPosZ()-6 == (rp->worldZ/ segmentLength) && e->enemyEnabled()) {
			roadPoint *roadP = roadPoints[((int)e->getPosZ()) % roadLength];
			if (App->player->collider->checkCollisionCoordX(e->collider->rect)) {
				if (App->player->getSpeed() > App->player->getMinSpeed()) {
					App->player->setSpeed(App->player->getMinSpeed());
				}
			}
		}
	}
}

void ModuleRoad::setUpEnding(bool gameOVer) {
	App->player->animateToIDLE();
	for (int i = 0; i < App->enemies->enemies.size(); ++i) {
		Enemy *e = App->enemies->enemies[i];
		if ((e->getPosZ() < (int)(camZPosition / segmentLength)) && (e->getPosZ() > (int)(camZPosition / segmentLength) + drawDistance)) {
			e->setEnabled(false);
		}
	}
	if (!gameOVer)	App->gui->switchGUImodeToScore(SCORES_MODE);
	else App->gui->switchGUImodeToScore(GAME_OVER_MODE);
}

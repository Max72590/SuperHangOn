#include "ModuleRoad.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleGUI.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"


ModuleRoad::ModuleRoad(bool active): Module(active){
}


ModuleRoad::~ModuleRoad(){
}


bool ModuleRoad::Start() {
	return true;
}


update_status ModuleRoad::Update(float deltaTime) {
	SDL_Color c = stageColors[colorIndex].sky;
	App->renderer->DrawQuad(SDL_Rect({0,0,640,480}), c.r,c.g,c.b,c.a);
	App->renderer->ScaledBlit(background, (int)backgroundPosX, landscapeY- sky[colorIndex]->h*2, sky[colorIndex], sky[colorIndex]->w * 2, sky[colorIndex]->h * 2);
	App->renderer->ScaledBlit(background, (int)foregroundPosX, landscapeY- foreground[colorIndex]->h*2, foreground[colorIndex], foreground[colorIndex]->w * 2, foreground[colorIndex]->h * 2);
	camZPosition = calculatePosZ(App->player->getSpeed()*deltaTime*40);
	bool actualSeg = (((int)(camZPosition / segmentLength)) >= endSegmentIndex);
	if (actualSeg && !crossedEndSegment) {
		crossedEndSegment = true;
		setUpEnding(false);
		runGameOverTimer = true;
	}
	if (stageColorChangeIndexes[stageIndex] <  (int)(camZPosition / segmentLength)) {

		if (colorIndex < (int)stageColorChangeIndexes.size()){		
			bool color1Ok = calculateColorTransition(stageColors[colorIndex].sky, stageColors[colorIndex+1].sky);
			bool color2Ok = calculateColorTransition(stageColors[colorIndex].roadDark, stageColors[colorIndex + 1].roadDark);
			bool color3Ok = calculateColorTransition(stageColors[colorIndex].roadLight, stageColors[colorIndex + 1].roadLight);
			bool color4Ok = calculateColorTransition(stageColors[colorIndex].offroadDark, stageColors[colorIndex + 1].offroadDark);
			bool color5Ok = calculateColorTransition(stageColors[colorIndex].offroadLight, stageColors[colorIndex + 1].offroadLight);
			if (color1Ok && color2Ok && color3Ok && color4Ok && color5Ok) {
				++colorIndex;
				startColorTransition = false;
				if (stageIndex < (int)stageColorChangeIndexes.size() - 1) ++stageIndex;
			}
		}
	}

	if (runTimer) {
		updateTimer(deltaTime);
	}
	else {
		if (semaphore.Finished()) {
			App->enemies->startRace();
			runTimer = true;
			App->player->activatePlayer(true);
		}
	}
	paintRoad(deltaTime);
	App->gui->updateGUIValues((runGameOverTimer? gameOverCountdown : raceSeconds), App->player->getScore(), (int)App->player->getSpeed());

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
	for (int i = 0; i < (int)sky.size(); ++i) {
		RELEASE (sky[i]);
		RELEASE (foreground[i]);
	}
	roadDecorations = nullptr;
	background = nullptr;
	sky.clear();
	foreground.clear();
	resetRoad();
	stageColorChangeIndexes.clear();
	stageColors.clear();
	App->player->Disable();
	App->enemies->Disable();
	App->collision->Disable();
	App->gui->Disable();
	return true;
}

void ModuleRoad::resetRoad() {
	camZPosition = 0;
	backgroundPosX = -128;
	foregroundPosX = 0;
	offsetX = 0;
	roadX = 0;
	runTimer = false;
	runGameOverTimer = false;
	timerAcum = 0.0f;
	raceSeconds = 50;
	gameOverCountdown = 10;
	colorIndex = 0;
	crossedEndSegment = false;
	startColorTransition = false;
}


void ModuleRoad::paintRoad(float deltaTime) {
	while (camZPosition >= roadLength*segmentLength) camZPosition -= roadLength*segmentLength;
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
	App->player->offsetX(-(rp->curvefactor*1.5f)*deltaTime);
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
	landscapeY = (int)ymax;
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

void ModuleRoad::drawTrack(roadPoint const *p1, roadPoint const *p2, bool const isColor1) const {
	assert(p1 != nullptr);
	assert(p2 != nullptr);
	if (p1 == nullptr || p2 == nullptr) {
		if (p1 == nullptr) LOG("Point 1 is null");
		if (p2 == nullptr) LOG("Point 2 is null");
		return;
	}
	Sint16 p1x = (Sint16)(p1->screenX);
	Sint16 p1w = (Sint16)p1->screenW;
	Sint16 p2x = (Sint16)p2->screenX;
	Sint16 p2w = (Sint16)p2->screenW;
	Sint16 y1 = (Sint16)p1->screenY;
	Sint16 y2 = (Sint16)p2->screenY;
	Sint16 rumbleWidth1 = (Sint16) (p1w*0.10f);
	Sint16 rumbleWidth2 = (Sint16) (p2w*0.10f);
	Sint16 roadX[4] = {p1x-p1w, p2x-p2w, p2x+p2w, p1x + p1w };
	Sint16 greenX[4] = { 0, 0,SCREEN_WIDTH,SCREEN_WIDTH };
	Sint16 leftRumbleX[4] = {p1x-p1w,p2x-p2w,p2x - p2w - rumbleWidth2,p1x-p1w - rumbleWidth1 };
	Sint16 rightRumbleX[4] = {p1x+p1w,p2x+p2w,p2x+p2w+ rumbleWidth2,p1x+p1w+ rumbleWidth1 };
	Sint16 y[4] = { y1,y2,y2,y1 };
	SDL_Color roadColor, offRoadColor, rumbleColor;
	if (isColor1) {
		roadColor = stageColors[colorIndex].roadLight; 
		offRoadColor = stageColors[colorIndex].offroadDark; 
		rumbleColor = WHITE;
	}
	else {
		roadColor = stageColors[colorIndex].roadDark; 
		offRoadColor = stageColors[colorIndex].offroadLight; 
		rumbleColor = RED;
	}
	filledPolygonRGBA(App->renderer->renderer, greenX, y, 4, offRoadColor.r, offRoadColor.g, offRoadColor.b, offRoadColor.a);
	filledPolygonRGBA(App->renderer->renderer, roadX, y, 4, roadColor.r, roadColor.g, roadColor.b, roadColor.a);
	filledPolygonRGBA(App->renderer->renderer, rightRumbleX, y, 4, rumbleColor.r, rumbleColor.g, rumbleColor.b, rumbleColor.a);
	filledPolygonRGBA(App->renderer->renderer, leftRumbleX, y, 4, rumbleColor.r, rumbleColor.g, rumbleColor.b, rumbleColor.a);
}

void ModuleRoad::drawSprites(int initPos) {
	assert(initPos >= 0);
	if (initPos < 0) {
		LOG("The value of the index of the point is below 0");
		return;
	}
	// Scenery
	for (int j = initPos + (int)drawDistance; j > initPos; --j) {
		roadPoint *roadP = roadPoints[j % roadLength];
		if (roadP->prop->spriteID != -1) {
			SDL_Rect sprite;
			if (roadP->prop->spriteID > -1) sprite = *sprites[roadP->prop->spriteID];
			else if (roadP->prop->spriteID == -2) sprite = semaphore.GetCurrentFrame();
			float spriteX = roadP->screenX + roadP->screenScale * roadP->prop->spriteXCoord * (SCREEN_WIDTH / 2);
			float spriteY = roadP->screenY;
			float spriteW = sprite.w * (roadP->screenW / segmentLength)*roadP->prop->scalefactor;
			float spriteH = sprite.h * (roadP->screenW / segmentLength)*roadP->prop->scalefactor;
			spriteX += spriteW * roadP->prop->spriteXCoord;
			spriteY += spriteH * (-1);
			float clipH = spriteY + spriteH - roadP->clipCoord;
			if (clipH < 0) clipH = 0;
			if (clipH >= spriteH) continue;
			sprite.h = (int)ceil(sprite.h - ((sprite.h * (clipH*2.0f)) / spriteH));
			App->renderer->ScaledBlit(roadP->prop->tex, (int)(spriteX - spriteW / 2), (int)spriteY, &sprite, (int)spriteW, (int)spriteH);
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
	int steps = abs( (int)ceil( (actualHeight - previousHeight) / amount) );
	for (int i = actualPos - steps; i < actualPos; ++i) {
		(*roadPoints[i]).worldY = acumulator;
		if (actualHeight > previousHeight) acumulator += amount;
		else  acumulator -= amount;
	}
}

void ModuleRoad::updateTimer(float deltaTime) {
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
				App->audio->StopMusic();
				App->fade->FadeToBlack((Module*)App->map_selec, this);
			}
		}
	}
}

float ModuleRoad::calculatePosZ(float speed) {
	realPosZ += (int)speed;
	if (realPosZ > 200) {
		int numSteps = (int)(realPosZ / segmentLength);
		if (!runGameOverTimer)App->player->addScore(segmentLength*numSteps);
		camZPosition += numSteps * segmentLength;
		realPosZ -= numSteps * segmentLength;
	}
	return camZPosition;
}

void ModuleRoad::checkCollisions(roadPoint *rp) {
	assert(rp != nullptr);
	if (rp == nullptr) {
		LOG("The roadpoint is null");
		return;
	}
	if (rp->prop->spriteID > -1 && rp->prop->collider != nullptr) {
		if (App->player->collider->checkCollisionCoordX(rp->prop->collider->rect)) 	App->player->setPlayerState(FALLING);
	}
	for (int k = 0; k < (int)App->enemies->enemies.size(); ++k) {
		Enemy *e = App->enemies->enemies[k];
		if (e->getPosZ()-6 <= (rp->worldZ/ segmentLength) && e->getPosZ() > (rp->worldZ / segmentLength) && e->enemyEnabled()) {
			roadPoint *roadP = roadPoints[((int)e->getPosZ()) % roadLength];
			if (App->player->collider->checkCollisionCoordX(e->collider->rect)) {
				if (App->player->getSpeed() > App->player->getMinSpeed()) 	App->player->setSpeed(0);
			}
		}
	}
}

void ModuleRoad::setUpEnding(bool gameOVer) {
	App->player->animateToIDLE();
	for (int i = 0; i < (int)App->enemies->enemies.size(); ++i) {
		Enemy *e = App->enemies->enemies[i];
		e->setEnabled(false);
	}
	if (!gameOVer)	App->gui->switchGUImodeToScore(SCORES_MODE);
	else App->gui->switchGUImodeToScore(GAME_OVER_MODE);
}

bool ModuleRoad::calculateColorTransition(SDL_Color &c1, SDL_Color &c2)
{
	for (int i = 0; i < 3; ++i) {
		Uint8 color, targetColor;
		switch (i) {
			case 0:
				color = c1.r;
				targetColor = c2.r;
				break;
			case 1:
				color = c1.g;
				targetColor = c2.g;
				break;
			case 2:
				color = c1.b;
				targetColor = c2.b;
				break;
		}
		if (color != targetColor) {
			int diff = abs(color - targetColor);
			if (diff > 10) {
				if (color < targetColor) {
					color += 10;
				}
				if (color > targetColor) {
					color -= 10;
				}
			}
			else color = targetColor;
		}
		switch (i) {
			case 0:
				c1.r = color;
				break;
			case 1:
				c1.g = color;
				break;
			case 2:
				c1.b = color;
				break;
		}
	}
	if (c2.r == c1.r && c2.g == c1.g && c2.b == c1.b) {
		return true;
	}
	return false;
}
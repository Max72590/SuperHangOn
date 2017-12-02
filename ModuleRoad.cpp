#include "ModuleRoad.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
using namespace std;

ModuleRoad::ModuleRoad(bool active): Module(active)
{
}


ModuleRoad::~ModuleRoad()
{
}

bool ModuleRoad::Start() {

	int hillheight = 0;
	bool rising = true;
	int hillCount = 0;
	int percent = 0;
	bool up;
	int hillCounter = 0;
	for (float i = 0; i < 5000; ++i) {
		roadPoint rp;
		rp.worldZ = i*segmentLength;
		// Curves
		if (i > 300 && i < 500) rp.curvefactor = 2;
		else if (i > 2000 && i < 2200) rp.curvefactor = -2;
		else if (i > 2400 && i < 2600) rp.curvefactor = 3;
		// Hills
		if (i > 0) {
			float cosinus = 0;
			/*if (i >= 450 && i <= 720) {
				cosinus = cos(i/30)*1000;
				//float diff = abs(roadPoints[i - 1].worldY - cosinus);
				//if (diff > 0.3) smoothInOut(i,diff);
				rp.worldY = cosinus;
			}
			else rp.worldY = roadPoints[i - 1].worldY;*/
		}
		else rp.worldY = 0;
		roadPoints.push_back(rp);

	}
	roadLength = roadPoints.size();
	camDepth = 0.84; // Horizon looks better with this value
	//camDepth = 0.58 
	sky.x = 0;
	sky.y = 0;
	sky.w = 640;
	sky.h = 480;
	background = App->textures->Load("rtype/blueBackground.png");
	roadAssets = App->textures->Load("rtype/sprites.png");
	SDL_Rect *tree = new SDL_Rect();
	tree->x = 1204;
	tree->y = 4;
	tree->w = 283;
	tree->h = 296;
	sprites.push_back(tree);
	App->player->Enable();
	playerX = 0;
	camZPosition = 0;
	roadLength = roadPoints.size();
	for (int j = 300; j < 500; ++j) {
		if ((j % 20) == 0 ) {
			roadPoints[j].spriteID = 0;
			roadPoints[j].spriteXCoord = -2.0;
		}
	}
	return true;
}

void ModuleRoad::smoothInOut(int position, float amount) {
	int step = (int) ceil(amount / 0.2);
	for (int i = position - step; i < position; ++i) {
		if (roadPoints[i].worldY > 0) roadPoints[i].worldY -= 0.2;
		else  roadPoints[i].worldY += 0.2;
	}
}


update_status ModuleRoad::Update() {
	App->renderer->Blit(background, 0, 0,&sky);
	camZPosition += 200;
	paintRoad();
	return UPDATE_CONTINUE;
}

bool ModuleRoad::CleanUp() {
	App->textures->Unload(background);
	App->player->Disable();
	return true;
}

void ModuleRoad::resetRoad() {

}


void ModuleRoad::paintRoad() {
	while (camZPosition >= roadLength*200) camZPosition -= roadLength*segmentLength;
	while (camZPosition < 0) camZPosition += roadLength*segmentLength;
	int ymax = SCREEN_HEIGHT;
	int initPos = (int)(camZPosition / segmentLength);
	roadPoint* rp = &roadPoints[initPos];
	camHeight = (int)(1500 + rp->worldY);
	offsetX = 0;
	roadX = 0;

	for (int i = initPos; i < initPos + drawDistance; ++i) {
		roadPoint *rpActual = &roadPoints[i%roadLength];
		projection(*rpActual, (i>= roadLength) );
		roadX += offsetX;
		offsetX += rpActual->curvefactor;
		rpActual->clipCoord = (float) ymax;
		if ( (rpActual->screenY >= ymax) ) continue;
		ymax = rpActual->screenY;
		(i == 0 ? ++i : i );
		roadPoint *rpPrevious = &roadPoints[(i - 1)%roadLength];
		drawTrack( (*rpPrevious), (*rpActual), ((i/3)%2 == 0 ));		
	}
	drawSprites(initPos);
}

void ModuleRoad::projection(roadPoint &rp, bool looped) {
	float camX = rp.worldX - (int)(playerX - roadX);
	float camY = rp.worldY - camHeight;
	float camZ = rp.worldZ - (camZPosition - (looped ? roadLength*segmentLength : 0));
	rp.screenScale = camDepth / camZ;
	rp.screenX = ( (1 + (rp.screenScale * camX) ) * (SCREEN_WIDTH / 2)) ;
	rp.screenY = ( (1 - (rp.screenScale * camY) ) * (SCREEN_HEIGHT/1.8)); //(SCREEN_HEIGHT / 2) ) );
	rp.screenW = (rp.screenScale*roadWidth * (SCREEN_WIDTH / 2) );
}

void ModuleRoad::drawTrack(roadPoint const &p1, roadPoint const &p2, bool const isColor1) {
	Sint16 p1x = p1.screenX;
	Sint16 p1w = p1.screenW;
	Sint16 p2x = p2.screenX;
	Sint16 p2w = p2.screenW;					
	Sint16 y1 = p1.screenY;
	Sint16 y2 = p2.screenY;
	Sint16 roadX[4] = {p1x-p1w, p2x-p2w, p2x+p2w, p1x + p1w };
	Sint16 greenX[4] = { 0, 0,SCREEN_WIDTH,SCREEN_WIDTH };
	Sint16 leftRumbleX[4] = {p1x-p1w,p2x-p2w,p2x - p2w -p2w*0.05,p1x-p1w -p1w*0.05 };
	Sint16 rightRumbleX[4] = {p1x+p1w,p2x+p2w,p2x+p2w+p2w*0.05,p1x+p1w+p1w*0.05 };
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
	for (int j = initPos + drawDistance; j > initPos; --j) {
		roadPoint *roadP = &roadPoints[j%roadLength];
		if (roadP->spriteID != -1) {
			SDL_Rect sprite = *sprites[roadP->spriteID];
			float spriteX = roadP->screenX + roadP->screenScale * roadP->spriteXCoord * (SCREEN_WIDTH / 2);
			float spriteY = roadP->screenY + 4;
			float spriteW = sprite.w * (roadP->screenW/ 266);
			float spriteH = sprite.h * (roadP->screenW / 266);
			spriteX += spriteW * roadP->spriteXCoord;
			spriteY += spriteH * (-1);
			float clipH = spriteY + spriteH - roadP->clipCoord;
			if (clipH < 0) clipH = 0;
			if (clipH >= spriteH) return;
			App->renderer->ScaledBlit(roadAssets,(int)spriteX- spriteW/2, (int)spriteY, &sprite ,spriteW, spriteH);
		}
	}
}
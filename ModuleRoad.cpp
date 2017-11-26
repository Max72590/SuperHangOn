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
	for (int i = 0; i < 2000; ++i) {
		roadPoint rp = roadPoint();
		rp.worldX = 0;
		rp.worldY = 0;
		rp.worldZ = i*segmentLength;

		if (i > 300 && i < 500) rp.curvefactor = 2;
		//else if (i > 500 && i < 700) rp.curvefactor = -2;
		
		else if (i > 800 && hillCount == 0) {
			if (rising) {
				hillheight = roadPoints[i - 1].worldY + 20;
				if (hillheight > 2000) {
					rising = false;
					rp.worldY = hillheight;
				}
				else rp.worldY = hillheight;
				
			}
			else {
				rp.worldY = roadPoints[i - 1].worldY - 5;
				if (rp.worldY  < 0) {
					rp.worldY = 0;
					++hillCount;
				}
			}
		}
		//else if (i > 1000 && i < 1200) rp.curvefactor = 2;
		else rp.curvefactor = 0;
		roadPoints.push_back(rp);
	}
	roadLength = roadPoints.size() ;
	
	//camDepth = 0.84;
	camDepth = 0.58;
	sky.x = 0;
	sky.y = 0;
	sky.w = 640;
	sky.h = 480;
	background = App->textures->Load("rtype/blueBackground.png");
	App->player->Enable();
	playerX = 0;
	camZPosition = 0;
	roadLength = roadPoints.size();

	return true;
}

update_status ModuleRoad::Update() {
	App->renderer->Blit(background, 0, 0,&sky);
	//camZPosition += 200;
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
		rpActual->clipCoord = ymax;
		if ( (rpActual->screenY >= ymax) ) continue;
		ymax = rpActual->screenY;
		(i == 0 ? ++i : i );
		roadPoint *rpPrevious = &roadPoints[(i - 1)%roadLength];
		drawTrack( (*rpPrevious), (*rpActual), ((i/3)%2 == 0 ));
		
	}
	/*for (int j = initPos + drawDistance; j > initPos; --j) {
		roadPoint *roadP = &roadPoints[j%roadLength];
		if (roadP->spriteID != 0) {
			float spriteScale = roadP->screenScale;
			float spriteX = roadP->screenX+(roadP->spriteXCoordOffset*roadWidth*spriteScale*(SCREEN_WIDTH / 2));
			float spriteY = roadP->screenY;
			float scaledWidth = roadP->screenScale*sprites[roadP->spriteID]->w;
			float scaledHeight = roadP->screenScale*sprites[roadP->spriteID]->h;
			App->renderer->Blit(roadAssets,spriteX,spriteY,);
		}
	}*/
}

void ModuleRoad::projection(roadPoint &rp, bool looped) {
	float camX = rp.worldX - (int)(playerX - roadX);
	float camY = rp.worldY - camHeight;
	float camZ = rp.worldZ - (camZPosition - (looped ? roadLength*segmentLength : 0));
	rp.screenScale = camDepth / camZ;
	rp.screenX = ( (1 + (rp.screenScale * camX) ) * (SCREEN_WIDTH / 2)) ;
	rp.screenY = ( (1 - (rp.screenScale * camY) ) * (SCREEN_HEIGHT/2)); //(SCREEN_HEIGHT / 2) ) );
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

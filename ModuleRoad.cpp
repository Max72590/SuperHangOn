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
	for (int i = 0; i < 500; ++i) {
		roadSegment rs = roadSegment();
		rs.point1.worldX = 0;
		rs.point1.worldY = 0;
		rs.point1.worldZ = i*segmentLength;
		rs.point2.worldZ = (i + 1)*segmentLength;
		if (i % 2 == 0) {
			rs.color = ROAD_DARK;
		}
		else {
			rs.color = ROAD_LIGHT;
		}
		roadSegments.push_back(rs);
	}
	roadLength = roadSegments.size() * segmentLength;
	camDepth = 0.84;
	sky.x = 0;
	sky.y = 0;
	sky.w = 640;
	sky.h = 480;
	background = App->textures->Load("rtype/blueBackground.png");
	App->player->Enable();
	playerX = 0;
	return true;
}

update_status ModuleRoad::Update() {
	App->renderer->Blit(background, 0, 0,&sky);
	//camZPosition += 1;
	paintRoad();
	int scroll_speed = 1;
	return UPDATE_CONTINUE;
}

bool ModuleRoad::CleanUp() {
	App->textures->Unload(background);
	App->player->Disable();
	return true;
}

void ModuleRoad::resetRoad() {

}

roadSegment* ModuleRoad::getRoadSegment(int index) {
	if (index < (int) roadSegments.size()) return &roadSegments[index];
	else return  &roadSegments[0];
}

void ModuleRoad::paintRoad() {
	int ymax = SCREEN_HEIGHT;
	roadSegment *rs = getRoadSegment(camZPosition);

	offsetX = -1000;
	camHeight += rs->point1.worldY;
	//int PlayerXOffset = SCREEN_WIDTH/2 - App->player->position.x;
	if (rs != nullptr) {
		for (int i = 0; i < drawDistance; ++i) {
			roadSegment *rs = getRoadSegment(camZPosition+i);
			if (((rs->point1.worldZ) - (camZPosition)) > 0) {
				projection(rs->point1);
				projection(rs->point2);
				if ( (rs->point2.screenY > ymax) ) continue;
				else {
					Sint16 p1x = rs->point1.screenX;
					Sint16 p1w = rs->point1.screenW;
					Sint16 p2x = rs->point2.screenX;
					Sint16 p2w = rs->point2.screenW;					
					Sint16 y1 = rs->point1.screenY;
					Sint16 y2 = rs->point2.screenY;
					Sint16 x[4] = { p1x,p2x,p2x+p2w,p1x+p1w };
					Sint16 y[4] = { y1,y2,y2,y1 };
					//Sint16 x[4] = {p1x-p1w, p2x-p2w, p1x+p1w, p2x+p2w, };
					//Sint16 y[4] = { y1,y2,y2,y1 };
					Sint16 greenX[4] = { 0, 0,SCREEN_WIDTH,SCREEN_WIDTH };
					Sint16 leftRumbleX[4] = {p1x,p2x,p2x-p2w*0.05,p1x-p1w*0.05 };
					Sint16 rightRumbleX[4] = {p1x+p1w,p2x+p2w,p2x+p2w+p2w*0.05,p1x+p1w+p1w*0.05 };
					if (i % 2 == 0) {
						drawPolygon(greenX, y, GREEN_LIGHT);
						drawPolygon(rightRumbleX, y, WHITE);
						drawPolygon(leftRumbleX, y, WHITE);
					}
					else { 
						drawPolygon(greenX, y, GREEN_DARK);
						drawPolygon(rightRumbleX, y, RED);
						drawPolygon(leftRumbleX, y, RED);
					}
					SDL_Color color = rs->color;
					drawPolygon(x,y, rs->color);
					
				}
				ymax = rs->point2.screenY;
			}
		}
	}
}

void ModuleRoad::projection(roadPoint &rp) {
	
	int camX = rp.worldX -(playerX- offsetX);
	int camY = rp.worldY - camHeight;
	int camZ = rp.worldZ - (camZPosition);
	rp.screenScale = camDepth / camZ;
	rp.screenX = ( (SCREEN_WIDTH /2)+ (rp.screenScale * camX * (SCREEN_WIDTH / 2) ) );
	rp.screenY = ((SCREEN_HEIGHT / 2) - (rp.screenScale * camY * (SCREEN_HEIGHT / 2))); //(SCREEN_HEIGHT / 2) ) );
	rp.screenW = (rp.screenScale*roadWidth * (SCREEN_WIDTH / 2) );
	int a = 1;
}

void ModuleRoad::drawPolygon(Sint16 x[4], Sint16 y[4], SDL_Color color) {
	
	filledPolygonRGBA(App->renderer->renderer, x, y, 4, color.r, color.g, color.b, color.a);
}

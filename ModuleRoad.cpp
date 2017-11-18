#include "ModuleRoad.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
using namespace std;

ModuleRoad::ModuleRoad()
{
}


ModuleRoad::~ModuleRoad()
{
}

bool ModuleRoad::Start() {
	for (int i = 0; i < 500; ++i) {
		roadSegment rs = roadSegment();
		rs.point1Z.worldX = 0;
		rs.point1Z.worldY = 0;
		rs.point1Z.worldZ = i*segmentLength;
		rs.point2Z.worldZ = (i + 1)*segmentLength;
		if (i % 2 == 0) {
			rs.color.r = 62;
			rs.color.g = 62;
			rs.color.b = 62;
			rs.color.a = 255;
		}
		else {
			rs.color.r = 100;
			rs.color.g = 100;
			rs.color.b = 100;
			rs.color.a = 255;
		}
		roadSegments.push_back(rs);
	}
	roadLength = roadSegments.size() * segmentLength;
	camDepth = 1/(tan(50)*(M_PI/180));
	Disable();
	return true;
}

update_status ModuleRoad::Update() {
	paintRoad();
	return UPDATE_CONTINUE;
}

bool ModuleRoad::CleanUp() {
	return true;
}

void ModuleRoad::resetRoad() {

}

roadSegment* ModuleRoad::getRoadSegment(int index) {
	if (index < (int) roadSegments.size()) return &roadSegments[index];
	else return nullptr;
}

void ModuleRoad::paintRoad() {
	int ymax = SCREEN_HEIGHT;
	roadSegment *rs = getRoadSegment(camZPosition);
	int PlayerXOffset = SCREEN_WIDTH/2 - App->player->position.x;
	if (rs != nullptr) {
		for (int i = 0; i < drawDistance; ++i) {
			roadSegment *loopRoadSegment = getRoadSegment(camZPosition+i);
			if (((loopRoadSegment->point1Z.worldZ) - (camZPosition)) > 0) {
				projection(loopRoadSegment->point1Z);
				projection(loopRoadSegment->point2Z);
				if (( loopRoadSegment->point1Z.camZ <= camDepth) || (loopRoadSegment->point2Z.camY > ymax) ) continue;
				else {
					Sint16 x1 = loopRoadSegment->point1Z.screenX;
					Sint16 x2 = x1 + loopRoadSegment->point1Z.screenW;
					Sint16 x3 = loopRoadSegment->point2Z.screenX;
					Sint16 x4 = x3 + loopRoadSegment->point2Z.screenW;
					Sint16 y1 = loopRoadSegment->point1Z.screenY;
					Sint16 y2 = loopRoadSegment->point2Z.screenY;
					Sint16 x[4] = { x1,x2,x3,x4 };
					Sint16 y[4] = { y1,y1,y2,y2 };
					SDL_Color color = loopRoadSegment->color;
					filledPolygonRGBA(App->renderer->renderer, x, y, 4, color.r, color.g, color.b, color.a);
				}
				ymax = loopRoadSegment->point2Z.screenY;
			}
		}
	}
}

void ModuleRoad::projection(roadPoint &rp) {
	rp.camX = (rp.worldX || 0) - (App->player->position.x * roadWidth);
	rp.camY = (rp.worldY || 0) - camHeight;
	rp.camZ = (rp.worldZ || 0) - (camZPosition);
	rp.screenScale = camDepth / rp.camZ;
	rp.screenX = rint( (SCREEN_WIDTH / 2) + (rp.screenScale * rp.camX * (SCREEN_WIDTH / 2) ) );
	rp.screenY = rint( (SCREEN_HEIGHT / 2) - (rp.screenScale * rp.camY *  (SCREEN_HEIGHT / 2) ) );
	rp.screenW = rint(rp.screenScale*roadWidth * (SCREEN_WIDTH / 2) );
}

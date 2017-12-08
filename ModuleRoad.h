#ifndef __MODULEROAD_H__
#define __MODULEROAD_H__

#include "Module.h"
#include "ModulePlayer.h"
#include "SDL\include\SDL2_gfxPrimitives.h"
#include <vector>
#include <math.h>
#include "ModuleEnemy.h"

#define WHITE SDL_Color({255,255,255,255})
#define RED SDL_Color({255,0,0,255})
#define ROAD_DARK SDL_Color({62,62,62,255})
#define ROAD_LIGHT SDL_Color({100,100,100,255})
#define GREEN_DARK SDL_Color({0,150,0,255})
#define GREEN_LIGHT SDL_Color({0,200,0,255})

struct roadPoint {
	float worldX, worldY, worldZ, screenScale, screenX, screenY, screenW, curvefactor, clipCoord, spriteXCoord;
	int spriteID;

	roadPoint() {
		worldX = worldY = worldZ = screenScale = screenX = screenY = screenW = curvefactor = clipCoord = 0; 
		spriteXCoord = -1.0f;
		spriteID = -1;
	}
};


class ModuleRoad :
	public Module
{
public:
	ModuleRoad(bool active = true);
	~ModuleRoad();

	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();
	void resetRoad();
	void paintRoad();
	void projection(roadPoint &rp, bool looped);
	void drawTrack(roadPoint const &p1, roadPoint const &p2, bool const isColor1);
	void drawSprites(int initPos);
	void smoothInOut(int startPos, float amount);
	float calculatePosZ(float speed);

public:
	SDL_Rect sky;
	SDL_Texture* background = nullptr;
	SDL_Texture* roadAssets = nullptr;
	std::vector<roadPoint> roadPoints;
	std::vector<SDL_Rect*> sprites;
	std::vector<Enemy> enemies;	// in order, the first is the farthest.

	//Road
	float roadWidth = 2000;
	float segmentLength = 200;
	float rumbleLength = 3;
	float segmentsToDraw = 300;
	float roadY = 0;
	int roadLength =0;
	//Camera projection vars
	int camHeight = 1500;
	float camDepth;
	float drawDistance = 300;
	float camZPosition =0;
	float offsetX = 0;
	float roadX = 0;
};

#endif // __MODULEROAD_H__
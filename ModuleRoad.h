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


struct roadProp {
	float spriteXCoord,scalefactor;
	int spriteID;
	Collider *collider = nullptr;
	SDL_Texture *tex = nullptr;
	roadProp() {
		spriteXCoord = -1.0f;
		spriteID = -1;
		scalefactor = 1;
	}
};

struct roadPoint {
	float worldX, worldY, worldZ, screenScale, screenX, screenY, screenW, curvefactor, clipCoord;
	roadProp *prop = nullptr;
	roadPoint() {
		worldX = worldY = worldZ = screenScale = screenX = screenY = screenW = curvefactor = clipCoord = 0; 
		prop = new roadProp();
	}
};

struct colors {
	SDL_Color sky;
	SDL_Color roadDark;
	SDL_Color roadLight;
	SDL_Color offroadDark;
	SDL_Color offroadLight;
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
	void paintRoad(float deltaTime);
	void projection(roadPoint &rp, bool looped);
	void drawTrack(roadPoint const *p1, roadPoint const *p2, bool const isColor1) const;
	void drawSprites(int initPos);
	void smoothInOut(int previousPos, int startPos, float amount);
	void updateTimer(float deltaTime);
	float calculatePosZ(float speed);
	void checkCollisions(roadPoint *rp);
	void setUpEnding(bool gameOver);
	bool calculateColorTransition(SDL_Color &c1, SDL_Color &c2);

public:
	Animation semaphore;
	std::vector<SDL_Rect*> sky;
	std::vector<SDL_Rect*> foreground;
	SDL_Texture* background = nullptr;
	SDL_Texture* roadDecorations = nullptr;
	SDL_Texture* roadSigns = nullptr;
	std::vector<roadPoint*> roadPoints;
	std::vector<SDL_Rect*> sprites;
	std::vector<Enemy*> enemies;	// in order, the first is the farthest.
	std::vector<colors> stageColors;	
	std::vector<int> stageColorChangeIndexes;
	// Background
	float backgroundPosX = 0;
	float foregroundPosX = 0;
	int landscapeY = 0;
	//Road
	float roadWidth = 2000;
	int segmentLength = 200;
	float rumbleLength = 3;
	float segmentsToDraw = 300;
	float roadY = 0;
	int roadLength =0;
	int endSegmentIndex;
	int colorIndex;
	int stageIndex;
	bool crossedEndSegment = false;
	//Camera projection vars
	int camHeight = 1500;
	float camDepth;
	float drawDistance = 300;
	float camZPosition =0;
	float realPosZ = 0;
	float offsetX = 0;
	float roadX = 0;
	// Other variables
	bool runTimer, runGameOverTimer;
	bool startColorTransition = false;
	float timerAcum;
	int raceSeconds, gameOverCountdown;
};

#endif // __MODULEROAD_H__
#ifndef __MODULEROAD_H__
#define __MODULEROAD_H__

#include "Module.h"
#include "ModulePlayer.h"
#include "SDL\include\SDL2_gfxPrimitives.h"
#include <vector>
#include <math.h>

struct roadPoint {
	int worldX, worldY, worldZ, camX, camY, camZ, screenScale, screenX, screenY, screenW;
};

struct roadSegment {
	roadPoint point1Z,point2Z;
	SDL_Color color;
};

class ModuleRoad :
	public Module
{
public:
	ModuleRoad();
	~ModuleRoad();

	bool Start();
	update_status Update();
	bool CleanUp();
	void resetRoad();
	roadSegment* getRoadSegment(int index);
	void paintRoad();
	void projection(roadPoint &rp);
	

public:

	std::vector<roadSegment> roadSegments;
	//Road
	int roadWidth = 2000;
	int segmentLength = 200;
	int rumbleLength = 3;
	int segmentsToDraw = 300;
	int roadLength;
	//Camera projection vars
	int fieldOfView = 100;
	int camHeight = 1000;
	int camDepth;
	int drawDistance = 300;
	int camZPosition =0;
};

#endif // __MODULEROAD_H__
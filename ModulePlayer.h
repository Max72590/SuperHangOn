#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "ModuleCollision.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

enum state {IDLE,LEANING_LEFT,LEANING_RIGHT, REVERSE_LEFT, REVERSE_RIGHT,END_LEFT,END_RIGHT};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation left;
	Animation right;
	Animation endLeft;
	Animation endRight;
	Animation reverseLeft;
	Animation reverseRight;
	Animation *animArray[7] = {&idle,&left,&right, &reverseLeft, &reverseRight, &endLeft, &endRight};
	iPoint position;
	bool destroyed = false;
	Collider *collider;
	state playerState;
	float speed, maxspeed, accel,breaking,decel,offRoadDecel,offRoadLimit;
};

#endif
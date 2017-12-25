#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "ModuleCollision.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

enum state {IDLE,LEANING_LEFT,LEANING_RIGHT, REVERSE_LEFT, REVERSE_RIGHT,END_LEFT,END_RIGHT,FALLING};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();
	float getSpeed() const;
	void setSpeed(float value);
	float getValueX() const;
	float getMinSpeed()const;
	void offsetX(float value);
	void setPlayerState(state playerState);

public:
	SDL_Texture *actualTex = nullptr;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* falling_anim = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation left;
	Animation right;
	Animation endLeft;
	Animation endRight;
	Animation reverseLeft;
	Animation reverseRight;
	Animation falling;
	Animation *animArray[8] = {&idle,&left,&right, &reverseLeft, &reverseRight, &endLeft, &endRight, &falling};
	iPoint position;
	Collider *collider = nullptr;
	state playerState;


private:
	float scaleFactor = 1.0f;
	bool destroyed = false;
	float playerX;
	float speed;
	float maxspeed = 240.0;	// get this set on *road*
	float minspeed = 80.0;
	float accel = maxspeed / 5;
	float braking = -maxspeed;
	float decel = -maxspeed / 5;
	float fallDecel = -maxspeed / 2;
	float offRoadDecel = -maxspeed / 2;
	float offRoadLimit = maxspeed / 4;

};

#endif
#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "ModuleCollision.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleRoad.h"


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
	int getScore() const;
	void addScore(int value);
	void offsetX(float value);
	state getPlayerState()const;
	void setPlayerState(state playerState);
	void animateToIDLE();
	void activatePlayer(bool activate);

public:
	SDL_Rect *playerStopped;
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
	Animation *animArray[8] = { &idle,&left,&right, &reverseLeft, &reverseRight, &endLeft, &endRight, &falling };
	iPoint position;
	Collider *collider = nullptr;
	state playerState;


private:
	float scaleFactor = 1.0f;
	bool destroyed = false;
	bool reachedEndLine,startRunning;
	float playerX;
	float speed;
	float maxspeed = 240.0f;	// get this set on *road*
	float minspeed = 60.0f;
	float accel = maxspeed / 5.0f;
	float braking = -maxspeed;
	float decel = -maxspeed / 5.0f;
	float fallDecel = -maxspeed / 2.0f;
	float offRoadDecel = -maxspeed / 20.0f;
	float offRoadLimit = maxspeed / 4.0f;
	float maxPlayerX = 4000;
	int score;
};

#endif
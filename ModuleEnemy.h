#ifndef __MODULEENEMY_H__
#define __MODULEENEMY_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "Animation.h"
#include <vector>

enum enemyState { E_IDLE, E_LEANING_LEFT, E_LEANING_RIGHT, E_REVERSE_LEFT, E_REVERSE_RIGHT, E_END_LEFT, E_END_RIGHT };

struct Enemy {
	enemyState enemyAnimState;
	Collider *collider;
	Animation *current_animation = nullptr;
	Animation idle;
	Animation left;
	Animation right;
	Animation endLeft;
	Animation endRight;
	Animation reverseLeft;
	Animation reverseRight;
	Animation *animArray[7] = { &idle,&left,&right, &reverseLeft, &reverseRight, &endLeft, &endRight };
	float enemyPosZ, enemyPosX;
	
	Enemy(float x, float z, bool colorEnemy);
	Enemy(const Enemy& p);
	~Enemy();
	void Update(float deltaTime);
	void setPos(float posX, float posZ);
	SDL_Rect* getActualAnimRect();
	float getPosZ();
	void setMoveAnim(float direction);
};

class ModuleEnemy :
	public Module
{
public:
	ModuleEnemy(bool active);
	~ModuleEnemy();
	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();
	void startRace();
	void drawEnemy(float x, float y, float scaleW, float scaleH, Enemy *e);
	void addEnemy(Enemy &e,float x, float y);

public:
	SDL_Texture* sprites = nullptr;
	std::vector<Enemy*> enemies;
	Enemy *greenEnemy;
	Enemy *yellowEnemy;

private:
	bool startRunning = true;

};

#endif 
#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

enum TYPE {ENEMY_PROJ,PLAYER,LASER,WALL,ENEMY, NONE};
					 
struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	TYPE colliderType;
	Collider(SDL_Rect rectangle) :
		rect(rectangle)
	{}
	void setPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	void setWidthHeight(int w, int h) {
		rect.w = w;
		rect.h = h;
	}
	bool checkCollision(const SDL_Rect& r) const;
	bool checkCollisionCoordX(SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	bool CleanUp();
	Collider* AddCollider(const SDL_Rect& rect, TYPE Type);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
};

#endif // __ModuleCollision_H__
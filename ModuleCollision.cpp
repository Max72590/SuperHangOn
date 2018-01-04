#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

bool CollisionMatrix[6][6] = { { false,true,false,true,false,false },
{ true, false, false, true, true,false },
{ false,false,false,true,true,false },
{ true, true, true,false,true,false },
{ false,true, true, true, false,false },
{ false,false,false,false,false,false } };

ModuleCollision::ModuleCollision()
{
	debug = false;
}

ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate(float deltaTime)
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update(float deltaTime)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debug = !debug;
	if(debug == true) DebugDraw();
	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);
	colliders.clear();
	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, TYPE Type)
{
	Collider* ret = new Collider(rect);
	ret->colliderType = Type;
	colliders.push_back(ret);
	return ret;
}

// -----------------------------------------------------

bool Collider::checkCollisionCoordX(SDL_Rect& r) const
{
	int minX1 = this->rect.x;
	int minX2 = r.x;
	int maxX1 = this->rect.x + this->rect.w;
	int maxX2 = r.x + r.w;
	return !(maxX1 < minX2 || minX1 > maxX2);
}

bool Collider::checkCollision(const SDL_Rect& r) const
{
	return ( ( (r.x + r.w > this->rect.x) && (r.x + r.w < this->rect.x+ this->rect.w) || (r.x > this->rect.x) && (r.x < this->rect.x + this->rect.w) ) && ( (r.y > this->rect.y) && (r.y < this->rect.y+this->rect.h) || (r.y+r.h > this->rect.y) && (r.y+r.h < this->rect.y + this->rect.h)) );
	return false;
}

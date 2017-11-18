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
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
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

update_status ModuleCollision::Update()
{
	// TODO 8: Check collisions between all colliders. 
	// After making it work, review that you are doing the minumum checks possible
	list<Collider*>::iterator it, it2;
	for (it = colliders.begin(); it != colliders.end(); ++it) {
		for (it2 = it; it2 != colliders.end(); ++it2) {
			
			if ((it != it2) && (*it)->CheckCollision((*it2)->rect) && CollisionMatrix[(*it)->colliderType][(*it2)->colliderType]) {
				App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
				if ((*it)->colliderType == LASER || (*it)->colliderType == PLAYER) (*it)->to_delete = true;
				if ((*it2)->colliderType == LASER || (*it2)->colliderType == PLAYER) (*it2)->to_delete = true;
			}
			//else App->renderer->DrawQuad((*it)->rect, 0, 0, 255, 80);
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

// Called before quitting
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

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if rectangle argument "r" if intersecting with "this->rect"
	return ( ( (r.x + r.w > this->rect.x) && (r.x + r.w < this->rect.x+ this->rect.w) || (r.x > this->rect.x) && (r.x < this->rect.x + this->rect.w) ) && ( (r.y > this->rect.y) && (r.y < this->rect.y+this->rect.h) || (r.y+r.h > this->rect.y) && (r.y+r.h < this->rect.y + this->rect.h)) );
	return false;
}

#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	laser = new Particle();
	laser->anim.frames.push_back({ 232, 103, 16, 12 });
	laser->anim.frames.push_back({ 249, 103, 16, 12 });
	laser->anim.speed = 0.3f;
	//App->collision->AddCollider(laser->anim.GetCurrentFrame(), LASER);

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	explosion = new Particle();
	explosion->anim.frames.push_back({ 274, 296, 33, 30 });
	explosion->anim.frames.push_back({ 313, 296, 33, 30 });
	explosion->anim.frames.push_back({ 346, 296, 33, 30 });
	explosion->anim.frames.push_back({ 382, 296, 33, 30 });
	explosion->anim.frames.push_back({ 419, 296, 33, 30 });
	explosion->anim.frames.push_back({ 457, 296, 33, 30 });
	explosion->anim.speed = 0.6f;
	//App->collision->AddCollider(explosion->anim.GetCurrentFrame(), NONE);

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);
	
	active.clear();
	delete laser;
	delete explosion;
	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleParticles::PreUpdate(float deltaTime)
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}
	return UPDATE_CONTINUE;
}

// Update all particle logic and draw them
update_status ModuleParticles::Update(float deltaTime)
{
	/*for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Particle* p = *it;

		p->Update();
		App->renderer->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));

	}*/

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	Particle *projectile = new Particle(particle);
	projectile->collider = App->collision->AddCollider(projectile->anim.GetCurrentFrame(), LASER);
	projectile->position = { x,y };
	active.push_back(projectile);
}

Particle::Particle()
{}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position)
{}

Particle::~Particle()
{
}

void Particle::Update()
{
	position = {position.x +5, position.y };
	collider->setPos(position.x, position.y);
	if (position.x > (App->player->position.x) + (SCREEN_WIDTH)) {
		to_delete = true;
		collider->to_delete = true;
	}	
	if (first_Update) { 
		soundFX = App->audio->LoadFx("rtype / laser.wav");
		first_Update = false;
	}
	if (to_delete) {
		
	}

}


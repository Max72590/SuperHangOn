#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleSceneSpace.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneSpace::ModuleSceneSpace(bool active) : Module(active)
{}

ModuleSceneSpace::~ModuleSceneSpace()
{}

// Load assets
bool ModuleSceneSpace::Start()
{
	LOG("Loading space scene");
	
	background = App->textures->Load("rtype/blueBackground.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();

	//App->audio->PlayMusic("rtype/stage1.ogg", 1.0f);
	return true;
}

// UnLoad assets
bool ModuleSceneSpace::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(background);
	App->player->Disable();
	App->collision->Disable();
	App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneSpace::Update(float deltaTime)
{
	// Move camera forward -----------------------------
	int scroll_speed = 1;

	//App->player->position.x += 1;
	//App->renderer->camera.x -= 3;
	
	// Draw everything --------------------------------------
	App->renderer->Blit(background, 0, 0, NULL);
	
	return UPDATE_CONTINUE;
}
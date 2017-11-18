#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	// idle animation (just the ship)
	idle.frames.push_back({ 91, 360, 29, 71 });
	idle.frames.push_back({ 129, 360, 29, 71 });
	idle.loop = true;
	idle.speed = 0.2;

	// move right
	right.frames.push_back({ 91, 360, 29, 71 });
	right.frames.push_back({ 129, 360, 29, 71 });
	right.frames.push_back({ 168, 362, 34, 69 });
	right.frames.push_back({ 206, 362, 34, 69 });
	right.frames.push_back({ 244, 370, 43, 61 });
	right.frames.push_back({ 295, 370, 43, 61 });
	right.frames.push_back({ 345, 378, 61, 53 });
	right.frames.push_back({ 410, 378, 61, 53 });

	right.loop = false;
	right.speed = 0.2f;

	// Move left
	left.frames.push_back({ 91, 280, 29, 71 });
	left.frames.push_back({ 129, 280, 29, 71 });
	left.frames.push_back({ 166, 282, 34, 69 });
	left.frames.push_back({ 204, 282, 34, 69 });
	left.frames.push_back({ 243, 290, 43, 61 });
	left.frames.push_back({ 294, 290, 43, 61 });
	left.frames.push_back({ 344, 298, 61, 53 });
	left.frames.push_back({ 409, 298, 61, 53 });
	left.loop = false;
	left.speed = 0.2f;

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	graphics = App->textures->Load("rtype/playerSprites.png");

	destroyed = false;
	position.x = 320;
	position.y = 409;
	collider = App->collision->AddCollider(idle.GetCurrentFrame(), PLAYER);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;
	if (!destroyed){ 
		/*if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.x -= speed;
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			position.x += speed;
		}*/

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			//position.y += speed;
			if (current_animation != &right)
			{
				right.Reset();
				current_animation = &right;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//position.y -= speed;
			if (current_animation != &left)
			{
				left.Reset();
				current_animation = &left;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			// TODO 6: Shoot a laser using the particle system		
			App->particles->AddParticle(*(App->particles->laser), position.x + current_animation->GetCurrentFrame().w / 2, position.y + current_animation->GetCurrentFrame().h / 2);
			App->audio->PlayFx(App->particles->laser->soundFX);

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			current_animation = &idle;
}
	// Draw everything --------------------------------------
	if (destroyed == false) {
		int middleX = position.x - (current_animation->GetCurrentFrame().w /2);
		int middleY = 480 - (current_animation->GetCurrentFrame().h);
		App->renderer->Blit(graphics, middleX, middleY, &(current_animation->GetCurrentFrame()));
		collider->SetPos(position.x, position.y);
	}
	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player

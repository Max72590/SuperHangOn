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
	right.loop = false;
	right.speed = 0.2f;

	// Reverse right
	reverseRight.frames.push_back({ 244, 370, 43, 61 });
	reverseRight.frames.push_back({ 295, 370, 43, 61 });
	reverseRight.frames.push_back({ 168, 362, 34, 69 });
	reverseRight.frames.push_back({ 206, 362, 34, 69 });
	reverseRight.frames.push_back({ 91, 360, 29, 71 });
	reverseRight.frames.push_back({ 129, 360, 29, 71 });
	reverseRight.loop = false;
	reverseRight.speed = 0.2f;

	// End right
	endRight.frames.push_back({ 345, 378, 61, 53 });
	endRight.frames.push_back({ 410, 378, 61, 53 });
	endRight.loop = true;
	endRight.speed = 0.2f;

	// Move left
	left.frames.push_back({ 91, 280, 29, 71 });
	left.frames.push_back({ 129, 280, 29, 71 });
	left.frames.push_back({ 166, 282, 34, 69 });
	left.frames.push_back({ 204, 282, 34, 69 });
	left.frames.push_back({ 243, 290, 43, 61 });
	left.frames.push_back({ 294, 290, 43, 61 });
	left.loop = false;
	left.speed = 0.2f;

	// Reverse left
	reverseLeft.frames.push_back({ 243, 290, 43, 61 });
	reverseLeft.frames.push_back({ 294, 290, 43, 61 });
	reverseLeft.frames.push_back({ 166, 282, 34, 69 });
	reverseLeft.frames.push_back({ 204, 282, 34, 69 });
	reverseLeft.frames.push_back({ 91, 280, 29, 71 });
	reverseLeft.frames.push_back({ 129, 280, 29, 71 });
	reverseLeft.loop = false;
	reverseLeft.speed = 0.2f;

	// End left
	endLeft.frames.push_back({ 344, 298, 61, 53 });
	endLeft.frames.push_back({ 409, 298, 61, 53 });
	endLeft.loop = true;
	endLeft.speed = 0.2f;

	playerState = IDLE;
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
update_status ModulePlayer::Update(float deltaTime)
{

	int speed = 1;
	if (!destroyed){ 
		switch (playerState)
		{
		case IDLE:
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				right.Reset();
				playerState = LEANING_RIGHT;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				left.Reset();
				playerState = LEANING_LEFT;
			}
			break;
		case LEANING_LEFT:
			if (current_animation->Finished()) playerState = END_LEFT;
			break;
		case LEANING_RIGHT:
			if (current_animation->Finished()) playerState = END_RIGHT;
			break;
		case REVERSE_LEFT:
			if (current_animation->Finished()) playerState = IDLE;
			break;
		case REVERSE_RIGHT:
			if (current_animation->Finished()) playerState = IDLE;
			break;
		case END_LEFT:
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE	&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
				reverseLeft.Reset();
				playerState = REVERSE_LEFT;
			}
			break;
		case END_RIGHT:
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE	&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
				reverseRight.Reset();
				playerState = REVERSE_RIGHT;
			}
			break;
		default:
			break;
		}
		current_animation = animArray[playerState];

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


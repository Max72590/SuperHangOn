#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	idle.frames.push_back({ 180, 558, 67, 147 });	// 180,878, 67,147
	idle.frames.push_back({ 256, 558, 67, 147 }); // 256,878, 67,147
	idle.loop = true;
	idle.speed = 0.5f;

	// move right
	right.frames.push_back({ 870, 720, 73, 137 }); // 858, 1214, 73, 137
	right.frames.push_back({ 794, 720, 73, 137 }); // 782, 1214, 73, 137
	right.frames.push_back({ 698, 736, 93, 127 }); // 686, 1230, 93, 127
	right.frames.push_back({ 596, 736, 93, 127 }); // 584, 1230, 93, 127
	right.loop = false;
	right.speed = 0.5f;

	// Reverse right
	reverseRight.frames.push_back({ 596, 736, 93, 127 }); // 686, 1230, 93, 127
	reverseRight.frames.push_back({ 698, 736, 93, 127 }); // 584, 1230, 93, 127
	reverseRight.frames.push_back({ 794, 720, 73, 137 }); // 858, 1214, 73, 137
	reverseRight.frames.push_back({ 870, 720, 73, 137 }); // 782, 1214, 73, 137
	reverseRight.loop = false;
	reverseRight.speed = 0.5f;

	// End right
	endRight.frames.push_back({ 462, 752, 127, 111 }); // 688,914,127,111 
	endRight.frames.push_back({ 332, 752, 127, 111 }); // 818,914,127,111 
	endRight.loop = true;
	endRight.speed = 0.5f;

	// Move left
	left.frames.push_back({ 334, 562, 73, 137 });  // 336,888, 73,137
	left.frames.push_back({ 410, 562, 73, 137 }); // 410,888, 73,137
	left.frames.push_back({ 486, 578, 93, 127 }); // 486,898, 93,127
	left.frames.push_back({ 588, 578, 93, 127 }); // 588,898, 93,127
	left.loop = false;
	left.speed = 0.5f;

	// Reverse left
	reverseLeft.frames.push_back({ 486, 578, 93, 127 }); // 486,898, 93,127
	reverseLeft.frames.push_back({ 588, 578, 93, 127 }); // 588,898, 93,127
	reverseLeft.frames.push_back({ 334, 562, 73, 137 });  // 336,888, 73,137
	reverseLeft.frames.push_back({ 410, 562, 73, 137 }); // 410,888, 73,137
	reverseLeft.loop = false;
	reverseLeft.speed = 0.5f;

	// End left
	endLeft.frames.push_back({ 688, 594, 127, 111 }); // 688,914,127,111 
	endLeft.frames.push_back({ 818, 594, 127, 111 }); // 818,914,127,111 
	endLeft.loop = true;
	endLeft.speed = 0.5f;

	playerState = IDLE;
	playerX = 0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	graphics = App->textures->Load("rtype/miscellaneous.png");

	destroyed = false;
	position.x = 320;
	position.y = 409;
	collider = App->collision->AddCollider(idle.GetCurrentFrame(), PLAYER);
	speed = 0;
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
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE	|| App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				reverseLeft.Reset();
				playerState = REVERSE_LEFT;
			}
			break;
		case END_RIGHT:
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT	|| App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
				reverseRight.Reset();
				playerState = REVERSE_RIGHT;
			}
			break;
		default:
			break;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE || App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			speed += decel*deltaTime;
			if (speed < 0) speed = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			speed += accel*deltaTime*1.5;
			if (speed > maxspeed) speed = maxspeed;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && playerState != IDLE && speed > 0) {
			playerX -= deltaTime * 2000 * (speed / maxspeed);
			//if (playerX < -2) playerX = -2;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && playerState != IDLE && speed > 0) {
			playerX += deltaTime * 2000 * (speed/maxspeed);
			//if (playerX < 2) playerX = 2;
		}
		/*if (playerX > 1 || playerX < -1) {
			if (speed > offRoadLimit) speed -= offRoadDecel;
		}*/
		current_animation = animArray[playerState];
}
	// Draw everything --------------------------------------
	if (destroyed == false) {
		int middleX = position.x - (current_animation->GetCurrentFrame().w /2);
		int middleY = 480 - (current_animation->GetCurrentFrame().h);
		//App->renderer->Blit(graphics, middleX, middleY, &(current_animation->GetCurrentFrame()));
		App->renderer->ScaledBlit(graphics, middleX, middleY, &(current_animation->GetCurrentFrame()), (current_animation->GetCurrentFrame()).w , (current_animation->GetCurrentFrame()).h );
		collider->SetPos(position.x, position.y);
	}
	return UPDATE_CONTINUE;
}


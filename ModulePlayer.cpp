#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleRoad.h"


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
	right.speed = 0.1f;

	// Reverse right
	reverseRight.frames.push_back({ 596, 736, 93, 127 }); // 686, 1230, 93, 127
	reverseRight.frames.push_back({ 698, 736, 93, 127 }); // 584, 1230, 93, 127
	reverseRight.frames.push_back({ 794, 720, 73, 137 }); // 858, 1214, 73, 137
	reverseRight.frames.push_back({ 870, 720, 73, 137 }); // 782, 1214, 73, 137
	reverseRight.loop = false;
	reverseRight.speed = 0.1f;

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
	left.speed = 0.1f;

	// Reverse left
	reverseLeft.frames.push_back({ 486, 578, 93, 127 }); // 486,898, 93,127
	reverseLeft.frames.push_back({ 588, 578, 93, 127 }); // 588,898, 93,127
	reverseLeft.frames.push_back({ 334, 562, 73, 137 });  // 336,888, 73,137
	reverseLeft.frames.push_back({ 410, 562, 73, 137 }); // 410,888, 73,137
	reverseLeft.loop = false;
	reverseLeft.speed = 0.1f;

	// End left
	endLeft.frames.push_back({ 688, 594, 127, 111 }); // 688,914,127,111 
	endLeft.frames.push_back({ 818, 594, 127, 111 }); // 818,914,127,111 
	endLeft.loop = true;
	endLeft.speed = 0.5f;

	// Falling
	falling.frames.push_back({ 13, 98, 93, 24 });  // 24
	falling.frames.push_back({ 242, 91, 93, 24 }); // 24
	falling.frames.push_back({ 687, 79, 101, 27 }); // 21
	falling.frames.push_back({ 911, 73, 104, 29 }); // 19
	falling.frames.push_back({ 1130, 71, 112, 29 }); // 19
	falling.frames.push_back({ 30, 191, 119, 29 }); // 19
	falling.frames.push_back({ 257, 190, 117, 30 }); // 18
	falling.frames.push_back({ 477, 185, 124, 28 }); // 20
	falling.frames.push_back({ 697, 182, 124, 29 }); // 21
	falling.frames.push_back({ 917, 177, 119, 30 }); // 25
	falling.frames.push_back({ 1136, 175, 131, 31 }); // 26
	falling.frames.push_back({ 36, 294, 135, 33 }); // 29
	falling.loop = false;
	falling.speed = 0.033f;


}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	graphics = App->textures->Load("sprites/miscellaneous.png");
	falling_anim = App->textures->Load("sprites/falls.png");
	playerStopped = new SDL_Rect({ 180, 558, 67, 147 });
	playerState = IDLE;
	playerX = 0;
	destroyed = false;
	position.x = 320;
	position.y = 409;
	collider = App->collision->AddCollider(idle.GetCurrentFrame(), PLAYER);
	speed = 0;
	score = 0;
	actualTex = graphics;
	reachedEndLine = false;
	startRunning = false;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(graphics);
	App->textures->Unload(falling_anim);
	delete playerStopped;
	playerStopped = nullptr;
	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float deltaTime)
{

	if (!destroyed && !reachedEndLine && startRunning) {
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
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				reverseLeft.Reset();
				playerState = REVERSE_LEFT;
			}
			break;
		case END_RIGHT:
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
				reverseRight.Reset();
				playerState = REVERSE_RIGHT;
			}
			break;
		case FALLING:
			if (speed > 0) speed += fallDecel*deltaTime;
			if (falling.Finished()) {
				speed = 0;
				falling.Reset();
				playerX = 0;
				playerState = IDLE;
			}
			break;
		default:
			break;
		}
		if (playerState != FALLING && !reachedEndLine) {
			if ((playerX > (maxPlayerX / 2) || playerX < (-maxPlayerX / 2)) && (speed > offRoadLimit)) {
				speed += offRoadDecel;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {				
				if (speed < maxspeed) speed += accel*deltaTime;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				if (speed > 0) speed += braking;
				if (speed < 0) speed = 0;
			}
			else {
				if (speed > minspeed) speed += decel*deltaTime;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && playerState != IDLE && speed > 0) {
				playerX -= deltaTime * 2000 * (speed / maxspeed);
				if (playerX < -maxPlayerX ) playerX = -maxPlayerX ;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && playerState != IDLE && speed > 0) {
				playerX += deltaTime * 2000 * (speed / maxspeed);
				if (playerX > maxPlayerX ) playerX = maxPlayerX ;
			}
		}
		current_animation = animArray[playerState];
	}
	if (reachedEndLine) {
		if (playerX > 0.0f) {
			playerX -= deltaTime * 2000 * (speed/maxspeed);
			if (playerX < 0) playerX = 0.0f;
		}
		else if (playerX < 0.0f) {
			playerX += deltaTime * 2000 * (speed/maxspeed);
			if (playerX > 0) playerX = 0.0f;
		}
		if (speed > 0) {
			speed += decel*deltaTime;
			if (speed < 0) speed = 0;
		}
	}
	// Draw everything --------------------------------------
	if (destroyed == false) {		
		if (playerState == FALLING) {
			actualTex = falling_anim;
			scaleFactor = 2.0f;
		}
		else {
			actualTex = graphics;
			scaleFactor = 1.0f;
		}
		if (!startRunning || speed <= 0) {
			scaleFactor = 1.0f;
			int scaledW = (int)(playerStopped->w*scaleFactor);
			int scaledH = (int)(playerStopped->h*scaleFactor);
			int middleX = position.x - (scaledW / 2);
			int middleY = SCREEN_HEIGHT - scaledH;
			actualTex = graphics;
			App->renderer->ScaledBlit(actualTex, middleX, middleY, playerStopped,scaledW, scaledH);
			collider->setPos(middleX, middleY);
		}
		else {
			int scaledW = (int)(current_animation->GetCurrentFrame().w*scaleFactor);
			int scaledH = (int)(current_animation->GetCurrentFrame().h*scaleFactor);
			int middleX = position.x - (scaledW/ 2);
			int middleY = SCREEN_HEIGHT - scaledH;
			App->renderer->ScaledBlit(actualTex, middleX, middleY, &(current_animation->GetCurrentFrame()), scaledW, scaledH);
			collider->setPos(middleX, middleY);
		}
	}
	return UPDATE_CONTINUE;
}

float ModulePlayer::getSpeed() const {
	return speed;
}

float ModulePlayer::getMinSpeed() const {
	return minspeed;
}

void ModulePlayer::setSpeed(float value) {
	speed = value;
}


float ModulePlayer::getValueX() const {
	return playerX;
}

void ModulePlayer::offsetX(float value) {
	playerX += value* (speed / maxspeed)*500;
}

state ModulePlayer::getPlayerState()const {
	return playerState;
}


void ModulePlayer::setPlayerState(state pState) {
	playerState = pState;
}

void ModulePlayer::animateToIDLE() {
	switch (playerState) {
	case END_LEFT:
		App->player->setPlayerState(REVERSE_LEFT);
		break;
	case END_RIGHT:
		App->player->setPlayerState(REVERSE_RIGHT);
		break;
	case LEANING_LEFT:
		App->player->setPlayerState(REVERSE_LEFT);
		break;
	case LEANING_RIGHT:
		App->player->setPlayerState(REVERSE_RIGHT);
		break;
	}
	reachedEndLine = true;
}

int  ModulePlayer::getScore() const{
	return score;
}

void ModulePlayer::addScore(int value) {
	score += value;
}

void ModulePlayer::activatePlayer(bool activate) {
	startRunning = activate;
}
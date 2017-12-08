#include "ModuleEnemy.h"
#include "Application.h"
#include "ModuleRender.h"


ModuleEnemy::ModuleEnemy(bool active)
{

}

ModuleEnemy::~ModuleEnemy()
{
}

bool ModuleEnemy::Start() {
	sprites = App->textures->Load("rtype/miscellaneous.png");
	greenEnemy = new Enemy(0, 0, true);
	yellowEnemy = new Enemy(0, 0, false);
	startRunning = true;
	return true;
}

update_status ModuleEnemy::Update(float deltaTime) {
	if (startRunning) {
		for (Enemy *e : enemies)	e->Update(deltaTime);
	}
	return UPDATE_CONTINUE;
}

bool ModuleEnemy::CleanUp() {
	App->textures->Unload(sprites);
	return true;
}

void Enemy::setPos(float posX, float posZ) {
	enemyPosX = posX;
	enemyPosZ = posZ;
}

void ModuleEnemy::startRace() {
	startRunning = true;
}

void ModuleEnemy::drawEnemy(float x, float y, float scaleW, float scaleH, Enemy *e) {

	int middleX = e->enemyPosX - (e->current_animation->GetCurrentFrame().w / 2);
	int middleY = 480 - (e->current_animation->GetCurrentFrame().h * 2);
	//App->renderer->Blit(sprites, middleX, middleY, &(e->current_animation->GetCurrentFrame()));

	//App->renderer->Blit(sprites, middleX, middleY, &(current_animation->GetCurrentFrame()));
	App->renderer->ScaledBlit(sprites, x, y, &(e->current_animation->GetCurrentFrame()), scaleW, scaleH);
	//e->collider->SetPos(x, y);
}

void ModuleEnemy::addEnemy(Enemy &e, float x, float y) {
	Enemy *enemy = new Enemy(e);
	enemy->setPos(x, y);
	enemies.push_back(enemy);
}

// Enemy struct & functions

Enemy::Enemy(float x, float z, bool greenColor) {
	enemyPosX = x;
	enemyPosZ = z;
	idle.frames.push_back({ 180, (greenColor ? 878 : 1038), 67, 147 });	// 180,878, 67,147
	idle.frames.push_back({ 256, (greenColor ? 878 : 1038), 67, 147 }); // 256,878, 67,147
	idle.loop = true;
	idle.speed = 0.5f;

	// move right
	right.frames.push_back({ 858, (greenColor ? 1214 : 1374), 73, 137 }); // 858, 1214, 73, 137
	right.frames.push_back({ 782, (greenColor ? 1214 : 1374), 73, 137 }); // 782, 1214, 73, 137
	right.frames.push_back({ 686, (greenColor ? 1230 : 1390), 93, 127 }); // 686, 1230, 93, 127
	right.frames.push_back({ 584, (greenColor ? 1230 : 1390), 93, 127 }); // 584, 1230, 93, 127
	right.loop = false;
	right.speed = 0.5f;

	// Reverse right
	reverseRight.frames.push_back({ 686, (greenColor ? 1230 : 1390), 93, 127 }); // 686, 1230, 93, 127
	reverseRight.frames.push_back({ 584, (greenColor ? 1230 : 1390), 93, 127 }); // 584, 1230, 93, 127
	reverseRight.frames.push_back({ 858, (greenColor ? 1214 : 1374), 73, 137 }); // 858, 1214, 73, 137
	reverseRight.frames.push_back({ 782, (greenColor ? 1214 : 1374), 73, 137 }); // 782, 1214, 73, 137
	reverseRight.loop = false;
	reverseRight.speed = 0.5f;

	// End right
	endRight.frames.push_back({ 450, (greenColor ? 1246 : 1406), 127, 111 }); // 688,914,127,111 
	endRight.frames.push_back({ 320, (greenColor ? 1246 : 1406), 127, 111 }); // 818,914,127,111 
	endRight.loop = true;
	endRight.speed = 0.5f;

	// Move left
	left.frames.push_back({ 334, (greenColor ? 888 : 1042), 73, 137 });  // 336,888, 73,137
	left.frames.push_back({ 410, (greenColor ? 888 : 1042), 73, 137 }); // 410,888, 73,137
	left.frames.push_back({ 486, (greenColor ? 898 : 1058), 93, 127 }); // 486,898, 93,127
	left.frames.push_back({ 588, (greenColor ? 898 : 1058), 93, 127 }); // 588,898, 93,127
	left.loop = false;
	left.speed = 0.5f;

	// Reverse left
	reverseLeft.frames.push_back({ 486, (greenColor ? 898 : 1058), 93, 127 }); // 486,898, 93,127
	reverseLeft.frames.push_back({ 588, (greenColor ? 898 : 1058), 93, 127 }); // 588,898, 93,127
	reverseLeft.frames.push_back({ 334, (greenColor ? 888 : 1058), 73, 137 });  // 336,888, 73,137
	reverseLeft.frames.push_back({ 410, (greenColor ? 888 : 1058), 73, 137 }); // 410,888, 73,137
	reverseLeft.loop = false;
	reverseLeft.speed = 0.5f;

	// End left
	endLeft.frames.push_back({ 688, (greenColor ? 914 : 1074), 127, 111 }); // 688,914,127,111 
	endLeft.frames.push_back({ 818, (greenColor ? 914 : 1074), 127, 111 }); // 818,914,127,111 
	endLeft.loop = true;
	endLeft.speed = 0.5f;

	current_animation = &idle;
	enemyAnimState = E_IDLE;
	enemyPosX = 0;
	enemyPosZ = 0;
}

Enemy::Enemy(const Enemy& p) : idle(p.idle), left(p.left), right(p.right), endLeft(p.endLeft), endRight(p.endRight),
reverseLeft(p.reverseLeft), reverseRight(p.reverseRight), enemyAnimState(p.enemyAnimState),
current_animation(p.current_animation) {}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime) {
	switch (enemyAnimState) {
	case E_LEANING_LEFT:
		if (current_animation->Finished()) enemyAnimState = E_END_LEFT;
		break;
	case E_LEANING_RIGHT:
		if (current_animation->Finished()) enemyAnimState = E_END_RIGHT;
		break;
	case E_REVERSE_LEFT:
		if (current_animation->Finished()) enemyAnimState = E_IDLE;
		break;
	case E_REVERSE_RIGHT:
		if (current_animation->Finished()) enemyAnimState = E_IDLE;
		break;
	}
	current_animation = animArray[enemyAnimState];
	//if (enemyPosZ > 250) enemyPosZ += 2;
	//else	enemyPosZ += 1;
	enemyPosZ += 1;
}

SDL_Rect* Enemy::getActualAnimRect() {
	return &current_animation->GetCurrentFrame();
}

float Enemy::getPosZ() {
	return enemyPosZ;
}

void Enemy::setMoveAnim(float direction) {
	if (enemyAnimState == E_LEANING_LEFT || enemyAnimState == E_END_LEFT) {
		if (direction >= 0) enemyAnimState = E_REVERSE_LEFT;
	}
	else if (enemyAnimState == E_LEANING_RIGHT || enemyAnimState == E_END_RIGHT) {
		if (direction <= 0) enemyAnimState = E_REVERSE_RIGHT;
	}
	else if (enemyAnimState == E_IDLE) {
		if (direction < 0) enemyAnimState = E_LEANING_LEFT;
		else if (direction > 0) enemyAnimState = E_LEANING_RIGHT;
	}
}

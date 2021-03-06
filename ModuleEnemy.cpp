#include "ModuleEnemy.h"
#include "Application.h"
#include "ModuleRender.h"


ModuleEnemy::ModuleEnemy(bool active) : Module(active)
{

}

ModuleEnemy::~ModuleEnemy()
{
}

bool ModuleEnemy::Start() {
	sprites = App->textures->Load("sprites/miscellaneous.png");
	greenEnemy = new Enemy(0, 0, true);
	yellowEnemy = new Enemy(0, 0, false);
	startRunning = false;
	secondAcum = 0.0f;
	turboSeconds = 3;
	return true;
}

update_status ModuleEnemy::Update(float deltaTime) {
	if (startRunning) {
		secondAcum += deltaTime;
		if (secondAcum > 1.0f) {
			secondAcum = 0.0f;
			if (turboSeconds > 0)--turboSeconds;			
		}
		for (int i = 0; i < (int)enemies.size(); ++i) {
			if (turboSeconds <= 0) enemies[i]->setSpeed(enemies[i]->normalSpeed);
			if (enemies[i]->enemyPosZ > 500 && !enemies[i]->teleportedForward) {
				enemies[i]->enemyPosZ += 150 * i;
				enemies[i]->teleportedForward = true;
			}
			enemies[i]->Update(deltaTime);
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleEnemy::CleanUp() {
	App->textures->Unload(sprites);
	for (int i = 0; i < (int)enemies.size(); ++i) {
		RELEASE(enemies[i]);
	}
	enemies.clear();
	delete greenEnemy;
	delete yellowEnemy;
	return true;
}


void ModuleEnemy::startRace() {
	startRunning = true;
}

void ModuleEnemy::drawEnemy(float x, float y, float scaleW, float scaleH, Enemy *e) {
	if (e == nullptr) {
		LOG("Parameter enemy is null");
		return;
	}
	int middleX = (int)e->enemyPosX - ((int)e->current_animation->GetCurrentFrame().w / 2);
	int middleY = 480 - ((int)e->current_animation->GetCurrentFrame().h * 2);
	App->renderer->ScaledBlit(sprites, (int)x, (int)y, &(e->current_animation->GetCurrentFrame()), (int)scaleW, (int)scaleH);
}

void ModuleEnemy::addEnemy(Enemy &e, float x, float y) {
	Enemy *enemy = new Enemy(e);
	enemy->setPos(x, y);
	enemies.push_back(enemy);
}

void ModuleEnemy::updateEnemyCollider(Enemy *e, int posx, int posy, int width, int height) {
	if (e == nullptr) {
		LOG("Parameter enemy is null");
		return;
	}
	e->collider->setPos(posx,posy);
	e->collider->setWidthHeight(width,height);
}


// Enemy struct & functions

Enemy::Enemy(float x, float z, bool greenColor) {
	enemyPosX = x;
	enemyPosZ = z;
	idle.frames.push_back({ 180, (greenColor ? 878 : 1038), 67, 147 });	
	idle.frames.push_back({ 256, (greenColor ? 878 : 1038), 67, 147 }); 
	idle.loop = true;
	idle.speed = 0.5f;

	// move right
	right.frames.push_back({ 858, (greenColor ? 1214 : 1374), 73, 137 });
	right.frames.push_back({ 782, (greenColor ? 1214 : 1374), 73, 137 }); 
	right.frames.push_back({ 686, (greenColor ? 1230 : 1390), 93, 127 }); 
	right.frames.push_back({ 584, (greenColor ? 1230 : 1390), 93, 127 }); 
	right.loop = false;
	right.speed = 0.1f;

	// Reverse right
	reverseRight.frames.push_back({ 686, (greenColor ? 1230 : 1390), 93, 127 }); 
	reverseRight.frames.push_back({ 584, (greenColor ? 1230 : 1390), 93, 127 }); 
	reverseRight.frames.push_back({ 858, (greenColor ? 1214 : 1374), 73, 137 });
	reverseRight.frames.push_back({ 782, (greenColor ? 1214 : 1374), 73, 137 }); 
	reverseRight.loop = false;
	reverseRight.speed = 0.1f;

	// End right
	endRight.frames.push_back({ 450, (greenColor ? 1246 : 1406), 127, 111 });  
	endRight.frames.push_back({ 320, (greenColor ? 1246 : 1406), 127, 111 }); 
	endRight.loop = true;
	endRight.speed = 0.5f;

	// Move left
	left.frames.push_back({ 334, (greenColor ? 888 : 1042), 73, 137 }); 
	left.frames.push_back({ 410, (greenColor ? 888 : 1042), 73, 137 }); 
	left.frames.push_back({ 486, (greenColor ? 898 : 1058), 93, 127 }); 
	left.frames.push_back({ 588, (greenColor ? 898 : 1058), 93, 127 }); 
	left.loop = false;
	left.speed = 0.1f;

	// Reverse left
	reverseLeft.frames.push_back({ 486, (greenColor ? 898 : 1058), 93, 127 }); 
	reverseLeft.frames.push_back({ 588, (greenColor ? 898 : 1058), 93, 127 }); 
	reverseLeft.frames.push_back({ 334, (greenColor ? 888 : 1058), 73, 137 });  
	reverseLeft.frames.push_back({ 410, (greenColor ? 888 : 1058), 73, 137 }); 
	reverseLeft.loop = false;
	reverseLeft.speed = 0.1f;

	// End left
	endLeft.frames.push_back({ 688, (greenColor ? 914 : 1074), 127, 111 }); 
	endLeft.frames.push_back({ 818, (greenColor ? 914 : 1074), 127, 111 });  
	endLeft.loop = true;
	endLeft.speed = 0.5f;

	current_animation = &idle;
	enemyAnimState = E_IDLE;
	enemyPosX = 0;
	enemyPosZ = 0;
	normalSpeed = 0.6f;
	turboSpeed = 2.0f;
	speed = turboSpeed;
}

Enemy::Enemy(const Enemy& p) : idle(p.idle), left(p.left), right(p.right), endLeft(p.endLeft), endRight(p.endRight),
reverseLeft(p.reverseLeft), reverseRight(p.reverseRight), speed(p.speed), turboSpeed(p.turboSpeed), normalSpeed(p.normalSpeed),
enemyAnimState(p.enemyAnimState),current_animation(p.current_animation) {}

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime) {
	if (enabled) {
		enemyPosZ += speed;
		switch (enemyAnimState) {
		case E_LEANING_LEFT:
			if (current_animation->Finished()) {
				current_animation->Reset();
				enemyAnimState = E_END_LEFT;
			}
			break;
		case E_LEANING_RIGHT:
			if (current_animation->Finished()) {
				current_animation->Reset();
				enemyAnimState = E_END_RIGHT;
			}
			break;
		case E_REVERSE_LEFT:
			if (current_animation->Finished()) {
				current_animation->Reset();
				enemyAnimState = E_IDLE;
			}
			break;
		case E_REVERSE_RIGHT:
			if (current_animation->Finished()) {
				current_animation->Reset();
				enemyAnimState = E_IDLE;
			}
			break;
		}
		current_animation = animArray[enemyAnimState];
	}
}

SDL_Rect* Enemy::getActualAnimRect() const {
	return &current_animation->GetCurrentFrame();
}

float Enemy::getPosZ()const {
	return enemyPosZ;
}

float Enemy::getPosX()const {
	return enemyPosX;
}

void Enemy::setPos(float posX, float posZ) {
	enemyPosX = posX;
	enemyPosZ = posZ;
}

void::Enemy::setSpeed(float value) {
	speed = value;
}

void Enemy::setEnabled(bool enable) {
	enabled = enable;
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
	current_animation = animArray[enemyAnimState];
}

void  ModuleEnemy::enableMovement(bool enabled){
	startRunning = enabled;
}

bool Enemy::enemyEnabled()const {
	return enabled;
}
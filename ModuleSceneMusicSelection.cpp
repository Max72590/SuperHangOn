#include "ModuleSceneMusicSelection.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"


ModuleSceneMusicSelection::ModuleSceneMusicSelection(bool active) : Module(active)
{
}


ModuleSceneMusicSelection::~ModuleSceneMusicSelection()
{
}

bool ModuleSceneMusicSelection::Start() {
	background = App->textures->Load("rtype/MusicMenuBackground.png");
	musicMenu = App->textures->Load("rtype/musicMenu.png");
	selectedOptionAnim.frames.push_back({0,335,565,50});
	selectedOptionAnim.frames.push_back({0,335,5,5}); // Small size to be out of sight
	selectedOptionAnim.speed = 0.05f;
	selectedOptionAnim.loop = true;
	pressStartAnim.frames.push_back({ 0,335,565,50 });
	pressStartAnim.frames.push_back({ 0,335,5,5 }); // Small size to be out of sight
	pressStartAnim.speed = 0.05f;
	pressStartAnim.loop = true;
	optionSelectedIndex = 0;
	App->audio->PlayMusic(musicPaths[optionSelectedIndex].c_str(), 0.0f);
	return true;
}

update_status ModuleSceneMusicSelection::Update(float deltaTime) {
	App->renderer->Blit(background, 0, 0, &(SDL_Rect({ 0,0,640,480 })));
	App->renderer->Blit(musicMenu, 31 , optionsYCoords[optionSelectedIndex], &selectedOptionAnim.GetCurrentFrame());
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (optionSelectedIndex <= 0) optionSelectedIndex = 3;
		else --optionSelectedIndex;
		App->audio->PlayMusic(musicPaths[optionSelectedIndex].c_str(), 0.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (optionSelectedIndex >= 3) optionSelectedIndex = 0;
		else ++optionSelectedIndex;
		App->audio->PlayMusic(musicPaths[optionSelectedIndex].c_str(),0.0f);
	}
	App->renderer->Blit(musicMenu, 153, 394, &pressStartAnim.GetCurrentFrame());
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->road, this);
		App->audio->LoadFx("GameFX/starting.wav");
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneMusicSelection::CleanUp() {
	return true;
}
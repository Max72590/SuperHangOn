#include "ModuleFonts.h"
#include "Application.h"
#include "ModuleRender.h"
using namespace std;


ModuleFonts::ModuleFonts(bool active) : Module(active)
{
}


ModuleFonts::~ModuleFonts()
{
}

bool ModuleFonts::Start() {
	texFont = App->textures->Load("rtype/miscellaneousMark2.png");
	lettersFont = App->textures->Load("rtype/gameFont.png");
	yellowFont = Font(571, 403 , 162, 18,10,true);
	whiteFont = Font(571, 421, 162, 18,10, true);
	redFont = Font(571, 439, 162, 18,10, true);
	greenFont = Font(571, 457, 162, 18,10, true);
	timeFont = Font(0,1500, 212, 30,10, true);
	letters = Font(0,0,9,234,26,false);
	return true;
}

update_status ModuleFonts::Update(float deltaTime) {
	return UPDATE_CONTINUE;
}

bool ModuleFonts::CleanUp() {
	App->textures->Unload(texFont);
	App->textures->Unload(lettersFont);
	return true;
}

void ModuleFonts::drawMessage(fontName fn, string message, int posX, int posY) {
	Font *f = nullptr;
	SDL_Texture *t = nullptr;
	switch (fn)
	{
	case COLOR_RED:
		f = &redFont;
		t = texFont;
		break;
	case COLOR_GREEN:
		f = &greenFont;
		t = texFont;
		break;
	case COLOR_YELLOW:
		f = &yellowFont;
		t = texFont;
		break;
	case COLOR_WHITE:
		f = &whiteFont;
		t = texFont;
		break;
	case TIME_FONT:
		f = &timeFont;
		t = texFont;
		break;
	case WHITE_LETTERS:
		f = &letters;
		t = lettersFont;
		break;
	}
	if (f != nullptr) {
		vector<SDL_Rect*> rects;
		f->getTextRects(message, rects);
		for (int i = 0; i < (int)rects.size(); ++i) {
			posX += rects[i]->w;
			App->renderer->Blit(t, posX, posY, rects[i]);
			delete rects[i];
		}
		
	} 
	else  LOG("ERROR: Not valid font selected");
}

// Font

Font::Font() {}

Font::Font(int posX, int posY, int Swidth, int Sheight, int numchars, bool horizontallyOriented)
	:coordX(posX), coordY(posY), width(Swidth), height(Sheight), numChars(numchars),
	horizontalOrientation(horizontallyOriented) {
	int index = 0;
	if (numChars > 10) {
		for (int i = 0; i < 52; ++i) {
			charToIndex[letters[i]] = index;
			++index;
			if (i == 26) index = 0;
		}
	}
	else {
		for (int i = 0; i < 10; ++i) {
			charToIndex[nums[i]] = index;
			++index;
		}
	}
}


Font::~Font() {}


void Font::getTextRects(string message, vector<SDL_Rect*> &rects) {
	int charWidth = width / numChars;
	int charHeigth = height / numChars;
	for (int i = 0; i < (int)message.size(); ++i) {
		if (horizontalOrientation) {
			rects.push_back(new SDL_Rect({ coordX + (charWidth*charToIndex[message[i]]), coordY , charWidth , height }));
		}
		else {
			rects.push_back(new SDL_Rect({ coordX, coordY + (charHeigth *charToIndex[message[i]]) , width , charHeigth }));
		}
	}
}

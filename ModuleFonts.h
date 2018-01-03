#pragma once

#include "Module.h"
#include <string>
#include <vector>
#include <map>
#include "ModuleTextures.h"



enum fontName { COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_WHITE, TIME_FONT, WHITE_LETTERS };

struct Font {
	char nums[10] = { '0','1','2','3','4','5','6','7','8','9' };
	char letters[52] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	std::map<char, int> charToIndex;
	int coordX,coordY,width,height,numChars;
	bool horizontalOrientation;
	Font();
	Font(int posX, int posY, int Swidth, int Sheight, int numchars, bool horizontallyOriented);
	~Font();
	void getTextRects(std::string message, std::vector<SDL_Rect*> &rects);
};

class ModuleFonts :
	public Module
{
public:
	ModuleFonts(bool active);
	~ModuleFonts();
	bool Start();
	update_status Update(float deltaTime);
	bool CleanUp();
	void drawMessage(fontName fn, std::string message,int posX ,int posY);

public:
	SDL_Texture *texFont = nullptr;
	SDL_Texture *lettersFont = nullptr;
	Font yellowFont;
	Font whiteFont;
	Font redFont;
	Font greenFont;
	Font timeFont;
	Font letters;
};


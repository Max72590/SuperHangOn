#pragma once
#include "ModuleRoad.h"
class ModuleAsiaStage :
	public ModuleRoad
{
public:
	ModuleAsiaStage(bool active = true);
	~ModuleAsiaStage();

	bool Start();

};


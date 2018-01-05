#ifndef __MODULEASIASTAGE_H__
#define __MODULEASIASTAGE_H__

#include "ModuleRoad.h"
class ModuleAsiaStage :
	public ModuleRoad
{
public:
	ModuleAsiaStage(bool active = true);
	~ModuleAsiaStage();

	bool Start();

};

#endif // __MODULEASIASTAGE_H__
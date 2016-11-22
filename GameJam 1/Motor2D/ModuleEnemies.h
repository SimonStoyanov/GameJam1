#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "j1Module.h"

class ModuleEnemies : public j1Module
{
public:
	ModuleEnemies();

	// Destructor
	virtual ~ModuleEnemies();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Update all enemies
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	

private:

public:
private:
};

#endif // __ENEMIES_H__
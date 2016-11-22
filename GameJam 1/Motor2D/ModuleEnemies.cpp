#include "ModuleEnemies.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

ModuleEnemies::ModuleEnemies()
{
	name.create("enemies");
}

ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Awake(pugi::xml_node &)
{
	return true;
}

bool ModuleEnemies::Update(float dt)
{
	return true;
}

bool ModuleEnemies::CleanUp()
{
	return true;
}

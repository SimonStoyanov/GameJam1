#include "Enemies.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

Enemies::Enemies() : j1Module()
{
	name.create("enemies");
}

Enemies::~Enemies()
{
}

bool Enemies::Awake(pugi::xml_node &)
{
	return true;
}

bool Enemies::CleanUp()
{
	return true;
}

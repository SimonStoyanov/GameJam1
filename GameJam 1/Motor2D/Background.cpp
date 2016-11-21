#include "Background.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

Background::Background() : j1Module()
{
	name.create("background");
}

Background::~Background()
{
}

bool Background::Awake(pugi::xml_node &)
{
	return true;
}

bool Background::CleanUp()
{
	return true;
}

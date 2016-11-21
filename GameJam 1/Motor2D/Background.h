#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "j1Module.h"
#include "Prefabs.h"

struct SDL_Texture;

class Background : public j1Module
{
public:
	Background();

	// Destructor
	virtual ~Background();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

private:

public:
	p2List<Prefab*>		prefabs;

private:
};

#endif // __BACKGROUND_H__
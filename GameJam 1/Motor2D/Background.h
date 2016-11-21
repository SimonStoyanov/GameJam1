#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "j1Module.h"

#include "SDL/include/SDL.h"
#include "p2Point.h"

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

class Prefab
{
	Prefab();
	Prefab(SDL_Texture* _texture, int x, int y, int w, int h, int pos_x, int pos_y);
	Prefab(SDL_Texture* _texture, SDL_Rect _rect, int pos_x, int pos_y);
	~Prefab();

	SDL_Texture* texture;
	SDL_Rect rect;
	iPoint pos;
	PhysBody* pb = nullptr;
};

#endif // __BACKGROUND_H__
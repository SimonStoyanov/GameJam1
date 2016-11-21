#ifndef __PREFABS_H__
#define __PREFABS_H__

#include "p2Defs.h"
#include "SDL/include/SDL.h"

#include "p2Point.h"
#include "ModulePhysics.h"
#include "j1Textures.h"
#include "j1App.h"

struct Sprite {
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	iPoint pos;

	Sprite()
	{
		rect = { 0,0,0,0 };
		pos = { -1,-1 };
	}
	Sprite(int x, int y, char* texture_path, SDL_Rect rect) : pos(x, y), rect(rect)
	{
		texture = App->tex->Load(texture_path);
	}
};

class Prefab
{
public:
	Prefab();
	Prefab(int pos_x, int pos_y, char* texture_path, SDL_Rect rect );
	~Prefab();

public:
	Sprite sprite;
	PhysBody* body = nullptr;
};

#endif //__PREFABS_H__
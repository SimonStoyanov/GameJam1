#ifndef __PREFABS_H__
#define __PREFABS_H__

#include "j1App.h"
#include "p2Defs.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class App;
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

#endif //__PREFABS_H__
#ifndef __PREFABS_H__
#define __PREFABS_H__

#include "p2Defs.h"
#include "SDL/include/SDL.h"

#include "p2Point.h"
#include "ModulePhysics.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1App.h"

struct Sprite 
{
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	iPoint pos;

	Sprite()
	{
		rect = { 0,0,0,0 };
		pos = { -1,-1 };
	}

	Sprite(int x, int y,const char* texture_path, SDL_Rect rect) : pos(x, y), rect(rect)
	{
		if(texture_path != nullptr && texture_path[0] != '\0')
			texture = App->tex->Load(texture_path);
	}

	Sprite(int x, int y, SDL_Texture* texture, SDL_Rect rect) : pos(x, y), rect(rect), texture(texture)	{	}
};

class Prefab
{
public:
	//Constructors
	Prefab();
	Prefab(int pos_x, int pos_y, const char* texture_path, SDL_Rect rect );
	Prefab(int pos_x, int pos_y, SDL_Texture* texture, SDL_Rect rect);
	Prefab(Prefab* prefab);

	//Destructor
	~Prefab();

	//Methods
	void CreateCollision(int width, int height, int cat, int mask);
	void CreateCollision(int radius, int cat, int mask);

	void CreateStaticCollision(int width, int height, int cat, int mask);
	void CreateStaticCollision(int radius, int cat, int mask);
	void CreateSensorCollision(int width, int height, int cat, int mask);
	void CreateSensorCollision(int radius, int cat, int mask);

	iPoint GetPosition() const;

	int FindAnimation(AnimTypes type);

public:
	Sprite sprite;
	p2List<Animation*> animations;
	PhysBody* pbody = nullptr;
};

#endif //__PREFABS_H__
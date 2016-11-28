#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include "j1App.h"
#include "p2Point.h"

class Prefab;
struct SDL_Rect;
struct SDL_Texture;

struct prefabAndPlat
{
	prefabAndPlat(){}
	prefabAndPlat(Prefab* _prefab, int _plat)
	{
		prefab = _prefab;
		plat = _plat;
	}
	Prefab* prefab;
	int plat;
};

class RandomGenerator
{
public:
	RandomGenerator(Prefab* prefab, int _max_x, int _min_x, int _max_y, int _min_y, int pb_w, int pb_h, int ground);
	~RandomGenerator();

	void CheckRand(int x, int y, int to_del);
	PhysBody* GetClosestPlat();
private:
	void SetRand(int x, int y);
	void Blit(int x, int to_del);

public:
	p2List<SDL_Rect> rects;
	SDL_Texture* texture = nullptr;
private:
	Prefab* prefab = nullptr;
	p2List<prefabAndPlat> to_blit;

	iPoint pos;
	int max_x;
	int min_x;
	int max_y;
	int min_y;

	int w;
	int h;

	bool start;

	int plat_number;
	int ground;
};

#endif // __RANDGEN_H__
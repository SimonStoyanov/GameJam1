#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include "j1App.h"
#include "p2Point.h"


class Prefab;

class RandomGenerator
{
public:
	RandomGenerator(int x, int y, Prefab* prefab, int _max_x, int _min_x, int _max_y, int _min_y);
	~RandomGenerator();

	void CheckRand(int x, int to_del);
private:
	void SetRand(int x);
	void AddX(int x);
	void Blit(int x, int to_del);

public:
	iPoint pos;
	int max_x;
	int min_x;
	int max_y;
	int min_y;

private:
	Prefab* prefab;
	p2List<Prefab*> to_blit;

};

#endif // __RANDGEN_H__
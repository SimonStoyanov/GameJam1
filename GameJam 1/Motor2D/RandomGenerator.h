#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include "j1App.h"
#include "p2Point.h"


class Prefab;

class RandomGenerator
{
public:
	RandomGenerator(Prefab* prefab, int _max_x, int _min_x, int _max_y, int _min_y, int pb_w, int pb_h);
	~RandomGenerator();

	void CheckRand(int x, int to_del);
private:
	void SetRand(int x);
	void Blit(int x, int to_del);

public:

private:
	Prefab* prefab;
	p2List<Prefab*> to_blit;

	iPoint pos;
	int max_x;
	int min_x;
	int max_y;
	int min_y;

	int w;
	int h;

	bool start;

};

#endif // __RANDGEN_H__
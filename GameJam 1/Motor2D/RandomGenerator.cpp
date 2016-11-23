#include "RandomGenerator.h"
#include "j1Render.h"
#include "Prefabs.h"
#include <random>
#include <iostream>
using namespace std;

RandomGenerator::RandomGenerator(Prefab* _prefab, int _max_x, int _min_x, int _max_y, int _min_y, int pb_w, int pb_h)
{
	max_x = _max_x;
	min_x = _min_x;
	max_y = _max_y;
	min_y = _min_y;
	prefab = _prefab;

	w = pb_w;
	h = pb_h;

	start = true;
}

RandomGenerator::~RandomGenerator()
{
}

void RandomGenerator::CheckRand(int x, int y, int to_del)
{
	if (start)
	{
		SetRand(x, y);
		start = false;
	}

	if(x >= pos.x)
	{
		Prefab* tmp = new Prefab(prefab);
		tmp->sprite.pos.x = pos.x;
		tmp->sprite.pos.y = pos.y;

		tmp->CreateStaticCollision(w, h, WORLD, PLAYER);
		tmp->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0);
		to_blit.add(tmp);

		SetRand(x, y);
	}

	Blit(x, to_del);
}

void RandomGenerator::SetRand(int x, int y)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> disx(x + min_x, x + max_x);
	pos.x = disx(gen);

	int maxy = y + max_y;
	if (maxy > 525)
		maxy = 525;

	uniform_int_distribution<> disy(y - min_y , maxy);
	pos.y = disy(gen);

	//LOG("%d   min %d  max %d", pos.y, y - min_y, maxy);
}


void RandomGenerator::Blit(int x, int to_del)
{
	for(int i = 0; i < to_blit.count();)
	{
		//App->render->Blit(prefab->sprite.texture, to_blit[i]->sprite.pos.x, to_blit[i]->sprite.pos.y, &prefab->sprite.rect);

		if(to_blit[i]->sprite.pos.x < -(to_del - x))
		{
			App->physics->DeleteObject(to_blit[i]->pbody);
			RELEASE(to_blit[i]);
			to_blit.del(to_blit.At(i));
		}
		i++;
	}
}



#include "RandomGenerator.h"
#include "j1Render.h"
#include "Prefabs.h"
#include <random>
#include <iostream>
using namespace std;

RandomGenerator::RandomGenerator(int x, int y, Prefab* _prefab, int _max_x, int _min_x, int _max_y, int _min_y)
{
	pos.x = x;
	pos.y = y;
	max_x = _max_x;
	min_x = _min_x;
	max_y = _max_y;
	min_y = _min_y;
	prefab = _prefab;

	SetRand(x);
}

RandomGenerator::~RandomGenerator()
{
}

void RandomGenerator::CheckRand(int x, int to_del)
{
	LOG("%d %d", x, pos.x);
	if(x >= pos.x)
	{
		LOG("Generating something");
		Prefab* tmp= new Prefab(prefab);
		tmp->sprite.pos.x = pos.x;
		tmp->sprite.pos.y = pos.y;
		tmp->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0);
		to_blit.add(tmp);

		AddX(x);
		SetRand(x);
	}

	Blit(x, to_del);
}

void RandomGenerator::SetRand(int x)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> disx(x + min_x, x + max_x);
	pos.x = disx(gen);

	uniform_int_distribution<> disy(min_y, max_y);
	pos.y = disy(gen);
}

void RandomGenerator::AddX(int x)
{
	max_x += x;
	min_x += x;
}

void RandomGenerator::Blit(int x, int to_del)
{
	for(int i = 0; i < to_blit.count();)
	{
		//App->render->Blit(prefab->sprite.texture, to_blit[i]->sprite.pos.x, to_blit[i]->sprite.pos.y, &prefab->sprite.rect);

		if(to_blit[i]->sprite.pos.x < to_del - x)
		{
			to_blit.del(to_blit.At(i));
		}
		i++;
	}
}

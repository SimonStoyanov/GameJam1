#include "RandomGenerator.h"
#include "j1Render.h"
#include "Prefabs.h"
#include <random>
#include <iostream>
#include "Player.h"
using namespace std;

RandomGenerator::RandomGenerator(Prefab* _prefab, int _max_x, int _min_x, int _max_y, int _min_y, int pb_w, int pb_h, int _ground)
{
	max_x = _max_x;
	min_x = _min_x;
	max_y = _max_y;
	min_y = _min_y;
	prefab = _prefab;

	w = pb_w;
	h = pb_h;

	ground = _ground;

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
		plat_number++;
		if (plat_number >= 4) //
			plat_number = 0;

		if (plat_number != 3)
		{
			Prefab* tmp = new Prefab(prefab);
			tmp->sprite.pos.x = pos.x;
			tmp->sprite.pos.y = pos.y;

			tmp->CreateStaticCollision(w, h, WORLD, PLAYER);
			tmp->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0);
			tmp->pbody->type = platform;

			b2Filter a;
			a.categoryBits = PLAYER;
			a.maskBits = BOSS;
			tmp->pbody->body->GetFixtureList()->SetFilterData(a);

			prefabAndPlat pap(tmp, plat_number);
			to_blit.add(pap);
		}
		else
		{
			Prefab* tmp = new Prefab(prefab);
			tmp->sprite.pos.x = pos.x;
			tmp->sprite.pos.y = ground;

			tmp->CreateStaticCollision(50, 150, PLAYER, PLAYER);
			tmp->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(ground)), 0);
			tmp->pbody->type = wall;

			b2Filter a;
			a.categoryBits = PLAYER;
			a.maskBits = PLAYER;
			tmp->pbody->body->GetFixtureList()->SetFilterData(a);

			prefabAndPlat pap(tmp, plat_number);
			to_blit.add(pap);
		}

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

	uniform_int_distribution<> disy(y-min_y, max_y);
	pos.y = disy(gen);
}


void RandomGenerator::Blit(int x, int to_del)
{
	for(int i = 0; i < to_blit.count(); i++)
	{
		int compensation = 0;
		int plat_x = 0;
		switch(to_blit[i].plat)
		{
		case 0:
			compensation = 40; //60
			break;
		case 1:
			compensation = 60; //80
			break;
		case 2:
			compensation = 35; //55
			break;
		case 3:
			compensation = 50; //100
			break;
		}

		if(to_blit[i].plat == 3)
			App->render->Blit(texture, to_blit[i].prefab->GetPosition().x - 40, to_blit[i].prefab->GetPosition().y - compensation, &rects[to_blit[i].plat]);
		else
			App->render->Blit(texture, to_blit[i].prefab->GetPosition().x, to_blit[i].prefab->GetPosition().y - compensation, &rects[to_blit[i].plat]);

		if(to_blit[i].prefab->sprite.pos.x < -(to_del - x))
		{
			if(to_blit[i].prefab->sprite.texture != nullptr)
				App->tex->UnLoad(to_blit[i].prefab->sprite.texture);

			App->physics->DeleteObject(to_blit[i].prefab->pbody);
			RELEASE(to_blit[i].prefab);
			to_blit.del(to_blit.At(i));
			i--;
		}
	}
}

PhysBody* RandomGenerator::GetClosestPlat()
{
	if (to_blit.count() != 0)
	{
		int dX0 = App->player->player->GetPosition().x + App->player->player->sprite.rect.w;
		int dX1 = to_blit[0].prefab->GetPosition().x;

		int dY0 = App->player->player->GetPosition().y;
		int dY1 = to_blit[0].prefab->GetPosition().y;

		int closest = 0;
		int cdistance = sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));

		for (int i = 0; i < to_blit.count(); i++)
		{
			int dX0 = App->player->player->GetPosition().x;
			int dX1 = to_blit[i].prefab->GetPosition().x;

			int dY0 = App->player->player->GetPosition().y;
			int dY1 = to_blit[i].prefab->GetPosition().y;

			int dis = sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));

			if (cdistance > dis && dis > 0)
			{
				cdistance = dis;
				closest = i;
			}
		}

		/*
		dX0 = App->player->player->GetPosition().x + App->player->player->sprite.rect.w;
		dX1 = to_blit[closest]->GetPosition().x;

		dY0 = App->player->player->GetPosition().y;
		dY1 = to_blit[closest]->GetPosition().y;

		App->render->DrawLine(dX0, dY0, dX1, dY1, 255, 255, 255, 255);
		*/

		return to_blit[closest].prefab->pbody;
	}
	return nullptr;
}



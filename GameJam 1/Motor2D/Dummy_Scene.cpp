#include "Dummy_Scene.h"
#include "p2Defs.h"
#include "j1App.h"
#include "Player.h"
#include "Prefabs.h"
#include "ModulePhysics.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Text.h"

Dummy::Dummy() : Scene()
{
}

Dummy::~Dummy()
{
}

bool Dummy::Start()
{
	App->player->LoadTextures();

	int posx, posy;
	grounds.add(new Prefab(430, 560, nullptr, NULLRECT));
	grounds[0]->CreateStaticCollision(860, 8, WORLD, PLAYER);
	grounds[0]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx+1290, 560, nullptr, NULLRECT));
	grounds[1]->CreateStaticCollision(860, 8, WORLD, PLAYER);
	grounds[1]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx + 1290, 560, nullptr, NULLRECT));
	grounds[2]->CreateStaticCollision(860, 8, WORLD, PLAYER);


	return true;
}

bool Dummy::Update(float dt)
{
	int posx, posy;
	grounds[1]->pbody->GetPosition(posx, posy);
	if (-App->render->camera.x > posx) {
		grounds[2]->pbody->GetPosition(posx, posy);
		grounds.add(new Prefab(posx + 1290, 560, nullptr, NULLRECT));
		grounds[3]->CreateStaticCollision(860, 8, WORLD, PLAYER);
		grounds.del(grounds.start);
	}
	return true;
}

void Dummy::Draw()
{
}

bool Dummy::CleanUp()
{
	for (p2List_item<Prefab*>* item = grounds.start; item != nullptr; item = item->next)
	{
		RELEASE(item->data);
	}
	return true;
}

bool Dummy::IsGroundBody(PhysBody* body)
{
	for(p2List_item<Prefab*>* item = grounds.start; item != nullptr; item = item->next)
	{
		if (item->data->pbody == body)
			return true;
	}
	return false;
}

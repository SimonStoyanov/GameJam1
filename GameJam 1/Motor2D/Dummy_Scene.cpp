#include "Dummy_Scene.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "Player.h"
#include "Prefabs.h"
#include "ModulePhysics.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Text.h"
#include "RandomGenerator.h"

Dummy::Dummy() : Scene()
{
}

Dummy::~Dummy()
{
}

bool Dummy::Start()
{
	// Player
	pugi::xml_document levelscene;
	pugi::xml_node levelconfig;

	char* buf;
	int size = App->fs->Load("LevelScene.xml", &buf);
	pugi::xml_parse_result result = levelscene.load_buffer(buf, size);
	RELEASE(buf);

	levelconfig = levelscene.child("levelConfig");

	App->player->LoadTextures();

	// Grounds
	int posx, posy;
	grounds.add(new Prefab(430, 560, nullptr, NULLRECT));
	grounds[0]->CreateStaticCollision(860, 8, WORLD, PLAYER);
	grounds[0]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx+1290, 560, nullptr, NULLRECT));
	grounds[1]->CreateStaticCollision(860, 8, WORLD, PLAYER);
	grounds[1]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx + 1290, 560, nullptr, NULLRECT));
	grounds[2]->CreateStaticCollision(860, 8, WORLD, PLAYER);

	// Random generators
	test_pref = new Prefab(0, 0, nullptr, NULLRECT);
	test_pref->CreateStaticCollision(50, 50, 1, 1);
	test_rand = new RandomGenerator(300, 300, test_pref, 100, 50, 100, 10);

	return true;
}

bool Dummy::Update(float dt)
{

	test_rand->CheckRand(-App->render->camera.x + 860, 100);

	int posx, posy;
	grounds[1]->pbody->GetPosition(posx, posy);

	if (-App->render->camera.x > posx) 
	{
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

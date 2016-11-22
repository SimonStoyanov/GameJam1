#include "Dummy_Scene.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "Player.h"
#include "Prefabs.h"
#include "ModulePhysics.h"
#include "j1Textures.h"
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
	grounds.add(new Prefab(1017, 560, "Spritesheets/background_sheet.png", { 0,752,2034,146 }));
	grounds[0]->CreateStaticCollision(2034, 8, WORLD, PLAYER);
	grounds[0]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx+3051, 560, grounds[0]->sprite.texture, { 0,752,2034,146 }));
	grounds[1]->CreateStaticCollision(2034, 8, WORLD, PLAYER);
	grounds[1]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx + 3051, 560, grounds[0]->sprite.texture, { 0,752,2034,146 }));
	grounds[2]->CreateStaticCollision(2034, 8, WORLD, PLAYER);

	// Random generators
	test_pref = new Prefab(0, 0, "", NULLRECT);
	test_rand = new RandomGenerator(test_pref, 400, 50, 560, 300, 100, 10);

	background = Sprite(0, 0, "Spritesheets/background_sheet.png", NULLRECT);

	return true;
}

bool Dummy::Update(float dt)
{
	test_rand->CheckRand(-App->render->camera.x + 1000, App->player->player->GetPosition().y, 1500);

	int posx, posy;
	grounds[1]->pbody->GetPosition(posx, posy);

	if (-App->render->camera.x > posx) 
	{
		grounds[2]->pbody->GetPosition(posx, posy);
		grounds.add(new Prefab(posx + 3051, 560, grounds[0]->sprite.texture, NULLRECT));
		grounds[3]->CreateStaticCollision(2034, 8, WORLD, PLAYER);
		grounds.del(grounds.start);
	}

	return true;
}

void Dummy::Draw()
{
	for (p2List_item<Prefab*>* item = grounds.start; item != nullptr; item = item->next)
	{	
		App->render->Blit(item->data->sprite.texture, item->data->GetPosition().x, 422, &item->data->sprite.rect);
	}

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

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
#include "j1Window.h"
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

	levelconfig = levelscene.child("LevelConfig");

	ground_rect = { levelconfig.child("parallax").child("ground").attribute("rect_x").as_int(0),levelconfig.child("parallax").child("ground").attribute("rect_y").as_int(0), levelconfig.child("parallax").child("ground").attribute("rect_w").as_int(0),levelconfig.child("parallax").child("ground").attribute("rect_h").as_int(0) };
	back_rect = { levelconfig.child("parallax").child("background").attribute("rect_x").as_int(0),levelconfig.child("parallax").child("background").attribute("rect_y").as_int(0), levelconfig.child("parallax").child("background").attribute("rect_w").as_int(0),levelconfig.child("parallax").child("background").attribute("rect_h").as_int(0) };
	forward_rect = { levelconfig.child("parallax").child("forward").attribute("rect_x").as_int(0),levelconfig.child("parallax").child("forward").attribute("rect_y").as_int(0), levelconfig.child("parallax").child("forward").attribute("rect_w").as_int(0),levelconfig.child("parallax").child("forward").attribute("rect_h").as_int(0) };
	back_speed = levelconfig.child("parallax").child("background").attribute("speed").as_float(1);
	for_speed = levelconfig.child("parallax").child("forward").attribute("speed").as_float(1);
	parallax_spritesheet = levelconfig.child("parallax").child("spritesheet").attribute("path").as_string("");

	App->player->LoadTextures();

	// Grounds
	int posx, posy;
	grounds.add(new Prefab(ground_rect.w / 2, App->win->height - ground_rect.h / 4, parallax_spritesheet.GetString(), ground_rect));
	grounds[0]->CreateStaticCollision(ground_rect.w, 8, WORLD, PLAYER);
	grounds[0]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx+ground_rect.w*1.5f-1, App->win->height - ground_rect.h / 4, grounds[0]->sprite.texture, ground_rect));
	grounds[1]->CreateStaticCollision(ground_rect.w, 8, WORLD, PLAYER);
	grounds[1]->pbody->GetPosition(posx, posy);
	grounds.add(new Prefab(posx + ground_rect.w*1.5f-1, App->win->height - ground_rect.h / 4, grounds[0]->sprite.texture, ground_rect));
	grounds[2]->CreateStaticCollision(ground_rect.w, 8, WORLD, PLAYER);

	// Random generators
	test_pref = new Prefab(0, 0, "", NULLRECT);
	test_rand = new RandomGenerator(test_pref, 400, 50, 560, 300, 100, 10);

	background = Sprite(0, 0, grounds[0]->sprite.texture, back_rect);
	forward = Sprite(0, 0, background.texture, forward_rect);

	return true;
}

bool Dummy::Update(float dt)
{
	// Random updater
	test_rand->CheckRand(-App->render->camera.x + 1000, App->player->player->GetPosition().y, 1500);

	// Ground creation --

	int posx, posy;
	grounds[1]->pbody->GetPosition(posx, posy);

	if (-App->render->camera.x > posx) 
	{
		grounds[2]->pbody->GetPosition(posx, posy);
		grounds.add(new Prefab(posx + ground_rect.w*1.5f-1, App->win->height - ground_rect.h / 4, grounds[0]->sprite.texture, ground_rect));
		grounds[3]->CreateStaticCollision(ground_rect.w, 8, WORLD, PLAYER);
		grounds.del(grounds.start);
	}

	// ------------------ 
	return true;
}

bool Dummy::PostUpdate()
{
	App->render->Blit(forward.texture, forward_min, App->win->height - forward.rect.h, &forward.rect, for_speed);
	App->render->Blit(forward.texture, forward_min + forward.rect.w, App->win->height - forward.rect.h, &forward.rect, for_speed);


	if (((int)(-App->render->camera.x*for_speed) > forward.rect.w*forward_count)) {
		forward_min += forward.rect.w;
		forward_count++;
	}
	return true;
}

void Dummy::Draw()
{
	App->render->Blit(background.texture, background_min, 0, &background.rect, back_speed);
	App->render->Blit(background.texture, background_min + background.rect.w, 0, &background.rect, back_speed);

	if (((int)(App->render->camera.x*back_speed) > background.rect.w * background_count)) {
		background_min += background.rect.w;
		background_count++;
	}

	for (p2List_item<Prefab*>* item = grounds.start; item != nullptr; item = item->next)
	{	
		App->render->Blit(item->data->sprite.texture, item->data->GetPosition().x, App->win->height-ground_rect.h, &item->data->sprite.rect);
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

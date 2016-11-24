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
#include "ModuleEnemies.h"
#include "FearBoss.h"

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
	farback_rect = { levelconfig.child("parallax").child("farbackground").attribute("rect_x").as_int(0),levelconfig.child("parallax").child("farbackground").attribute("rect_y").as_int(0), levelconfig.child("parallax").child("farbackground").attribute("rect_w").as_int(0),levelconfig.child("parallax").child("farbackground").attribute("rect_h").as_int(0) };
	back_speed = levelconfig.child("parallax").child("background").attribute("speed").as_float(1);
	for_speed = levelconfig.child("parallax").child("forward").attribute("speed").as_float(1);
	farback_speed = levelconfig.child("parallax").child("farbackground").attribute("speed").as_float(1);
	parallax_spritesheet = levelconfig.child("parallax").child("spritesheet").attribute("path").as_string("");

	test_boss = (FearBoss*)App->enemies->CreateEnemy(fear);


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

	// Random generators ---

	platform = new Prefab(0, 0, "", NULLRECT);
	platforms_rand = new RandomGenerator(platform, 1000, 200, 460, 8, 290, 10);
	platforms_rand->texture = App->tex->Load("Spritesheets/platforms_sheet.png");
	platforms_rand->rects.add({0, 0, 350, 111});
	platforms_rand->rects.add({0, 111, 325, 156 });
	platforms_rand->rects.add({ 350, 56, 300, 193 });

	// ---------------------


	background = Sprite(0, 0, grounds[0]->sprite.texture, back_rect);
	forward = Sprite(0, 0, background.texture, forward_rect);

	return true;
}

bool Dummy::Update(float dt)
{
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
	App->render->Blit(background.texture, farback_min, 0, &farback_rect, farback_speed);
	App->render->Blit(background.texture, farback_min + farback_rect.w, 0, &farback_rect, farback_speed);

	if (((int)(-App->render->camera.x*farback_speed) > background.rect.w * farback_count)) {
		farback_min += background.rect.w;
		farback_count++;
	}
	
	App->render->Blit(background.texture, background_min, 0, &background.rect, back_speed);
	App->render->Blit(background.texture, background_min + background.rect.w, 0, &background.rect, back_speed);

	if (((int)(-App->render->camera.x*back_speed) > background.rect.w * background_count)) {
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

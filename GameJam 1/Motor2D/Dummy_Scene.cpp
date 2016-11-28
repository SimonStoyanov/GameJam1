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
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Text.h"
#include "RandomGenerator.h"
#include "ModuleEnemies.h"
#include "FearBoss.h"
#include "LoseScene.h"
#include <stdlib.h>
#include <time.h>

#define NUM_BOSSES 2

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

	//random boss

	srand(time(NULL));
	int rand_boss = rand() % NUM_BOSSES;

	switch (rand_boss)
	{
	case 0:
		test_boss = App->enemies->CreateEnemy(insanity);
		break;
	case 1:
		test_boss = App->enemies->CreateEnemy(fear);
		break;
	default:
		test_boss = App->enemies->CreateEnemy(insanity);
		break;
	}

	test_boss->prefab->pbody->listener = App->enemies;
	
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
	platforms_rand = new RandomGenerator(platform, 1100, 300, 460, 8, 240, 10, 430);
	platforms_rand->texture = App->tex->Load("Spritesheets/platforms_sheet.png");
	platforms_rand->rects.add({ 0, 0, 262, 83 });
	platforms_rand->rects.add({0, 83, 244, 117 });
	platforms_rand->rects.add({ 262, 42, 225, 145 });
	platforms_rand->rects.add({ 0, 195, 183, 193 });

	// ---------------------


	background = Sprite(0, 0, grounds[0]->sprite.texture, back_rect);
	forward = Sprite(0, 0, background.texture, forward_rect);

	forward_count = 1;
	background_count = 1;
	farback_count = 1;

	forward_min = 0;
	background_min = 0;
	farback_min = 0;

	App->audio->PlayMusic("audio/music/Music.ogg");

	//Start shapeball timer
	shapeball_timer.Start();
	App->spellmanager->CreateSpell(shapeball);

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

	//Create shape ball every 30 sec
	if (shapeball_timer.ReadSec() > 30) {
		App->spellmanager->CreateSpell(shapeball);
		shapeball_timer.Start();
	}

	// When player can jump
	for (int i = 0; i < 3; i++)
	{
		if (App->player->isTouching(App->player->player->pbody, grounds[i]->pbody))
		{
			App->player->on_ground = true;
		}
	}
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

	if (App->player->curr_hp <= 0) {
		switch (test_boss->type)
		{
		case fear:
			App->scene->have_fear = true;
			break;
		case insanity:
			App->scene->crazy = true;
			break;
		default:
			break;
		}
		App->scene->ChangeScene(App->scene->lose_scene);
	}

	int i = 0;
	SDL_Rect bossfullhp = { 46,227,20,46 }; 
	SDL_Rect bossemptyhp = { 20,227,20,46 };
	if (App->scene->have_fear)
	{
		for (; i < test_boss->max_hp; i++) {
			App->render->Blit(App->player->UI_texture, test_boss->prefab->GetPosition().x + i * 20 - ((test_boss->max_hp / 4) * 20), test_boss->prefab->GetPosition().y - 70, &bossfullhp);
		}
	}
	else {
		for (; i < test_boss->curr_hp; i++) {
			App->render->Blit(App->player->UI_texture, test_boss->prefab->GetPosition().x + i * 20 - ((test_boss->max_hp / 4) * 20), test_boss->prefab->GetPosition().y - 70, &bossfullhp);
		}
		for (; i < test_boss->max_hp; i++) {
			App->render->Blit(App->player->UI_texture, test_boss->prefab->GetPosition().x + i * 20 - ((test_boss->max_hp / 4) * 20), test_boss->prefab->GetPosition().y - 70, &bossemptyhp);
		}
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
	App->player->Disable();
	for (p2List_item<Prefab*>* item = grounds.start; item != nullptr; item = item->next)
	{
		App->physics->DeleteObject(item->data->pbody);
		RELEASE(item->data);
	}
	grounds.clear();
	App->enemies->Delete(test_boss);
	App->audio->StopMusic();
	App->spellmanager->CleanUp();
	App->render->camera.x = 0;
	App->physics->CleanBodies();
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

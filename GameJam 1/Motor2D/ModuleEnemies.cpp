#include "ModuleEnemies.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "Boss.h"
#include "FearBoss.h"
#include "InsanityBoss.h"
#include "WinScene.h"
#include "Player.h"

ModuleEnemies::ModuleEnemies()
{
	name.create("enemies");
}

ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Awake(pugi::xml_node &)
{
	char* buf;
	int size = App->fs->Load("EnemyConfig.xml", &buf);
	enemies_config.load_buffer(buf, size);
	RELEASE(buf);
	enemies_node = enemies_config.child("enemies");
	return true;
}

bool ModuleEnemies::Update(float dt)
{
	bool ret = true;

	if (App->scene->dummy_scene == App->scene->current_scene) // <----------
	{
		for (p2List_item<Boss*>* enemy = enemies.start; enemy != nullptr; enemy = enemy->next)
		{
			ret = enemy->data->Update(dt);
			//check boss dead
			if (enemy->data->curr_hp <= 0) {
				switch (enemy->data->type)
				{
				case fear:
					App->scene->have_fear = false;
					break;
				case insanity:
					App->scene->crazy = false;
					break;
				default:
					break;
				}
				App->scene->dummy_scene->round += 1;
				App->scene->ChangeScene(App->scene->win_scene);
			}
		}
	}
	return ret;
}

bool ModuleEnemies::PostUpdate()
{
	for (p2List_item<Boss*>* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
		enemy->data->Draw();
	}
	return true;
}

bool ModuleEnemies::CleanUp()
{
	for (p2List_item<Boss*>* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
		App->physics->DeleteObject(enemy->data->prefab->pbody);
		App->tex->UnLoad(enemy->data->prefab->sprite.texture);
		enemy->data->prefab->animations.clear();
		enemies.del(enemy);
	}
	return true;
}

void ModuleEnemies::Delete(Boss* boss)
{
	for (p2List_item<Boss*>* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) 
	{
		if (enemy->data == boss) 
		{
			App->physics->DeleteObject(enemy->data->prefab->pbody);
			App->tex->UnLoad(enemy->data->prefab->sprite.texture);
			enemy->data->prefab->animations.clear();
			enemies.del(enemy);
			break;
		}
	}
	enemies.clear();
}

Boss * ModuleEnemies::CreateEnemy(EnemiesTypes type)
{
	Boss* enemy = nullptr;
	switch (type)
	{
	case fear:
		enemy = new FearBoss(enemies_node.child("fear"));
		enemy->Start();
		break;
	case insanity:
		enemy = new InsanityBoss(enemies_node.child("insanity"));
		enemy->Start();
		break;
	case unknow:
		break;
	default:
		break;
	}
	enemies.add(enemy);
	return enemy;
}

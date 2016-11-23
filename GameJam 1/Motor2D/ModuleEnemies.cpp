#include "ModuleEnemies.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "Boss.h"
#include "FearBoss.h"

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
	for (p2List_item<Boss*>* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
		//enemy->data->Update();
		enemy->data->Draw();
	}
	return true;
}

bool ModuleEnemies::CleanUp()
{
	return true;
}

Boss * ModuleEnemies::CreateEnemy(EnemiesTypes type)
{
	Boss* enemy = nullptr;
	switch (type)
	{
	case fear:
		enemy = new FearBoss(enemies_node.child("fear"));
		break;
	case unknow:
		break;
	default:
		break;
	}
	enemies.add(enemy);
	return enemy;
}

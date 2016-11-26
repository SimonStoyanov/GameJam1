#include "fearball.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Render.h"
#include "Player.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"
#include "Dummy_Scene.h"
#include "Boss.h"
#include "ModuleEnemies.h"

Fearball::Fearball(pugi::xml_node& config) : Spell(fearball, "fearball")
{
	prefab = new Prefab(App->scene->dummy_scene->test_boss->prefab->GetPosition().x, App->scene->dummy_scene->test_boss->prefab->GetPosition().y, "", NULLRECT);

	prefab->LoadAnimations(config);
	curr_anim = prefab->FindAnimation(Idle);

	draw_offset.x = config.child("drawoffset").attribute("x").as_int(0);
	draw_offset.y = config.child("drawoffset").attribute("y").as_int(0);
	size.x = config.child("size").attribute("w").as_int(10);
	size.y = config.child("size").attribute("h").as_int(10);

}

Fearball::~Fearball()
{
}

void Fearball::Start()
{
	prefab->CreateCollision(size.x, size.y, WORLD, PLAYER);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);
	if (prefab->pbody != nullptr) {
		iPoint vel;
		int player_x, player_y;
		App->player->player->pbody->GetPosition(player_x, player_y);
		int boss_x, boss_y;
		for (p2List_item<Boss*>* boss = App->enemies->enemies.start; boss != nullptr; boss = boss->next) {
			if (boss->data->shoot) {
				boss->data->prefab->pbody->GetPosition(boss_x, boss_y);
				break;
			}
		}
		float delta_x = -player_x + boss_x;
		float delta_y = -player_y + boss_y;
		float alpha = atan(delta_y / delta_x);

		vel.x = fearball_speed*cos(alpha);
		vel.y = fearball_speed*sin(alpha);

		prefab->pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	}
}

bool Fearball::Update()
{
	if (collided) {
		if (prefab->pbody->body->GetLinearVelocity().x != 0 || prefab->pbody->body->GetLinearVelocity().y != 0)
			prefab->pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		if (prefab->animations[curr_anim]->Finished()) {
			to_delete = true;
		}
	}
	return true;
}

void Fearball::Draw()
{
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x + draw_offset.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[curr_anim]->GetCurrentFrameRect());
}

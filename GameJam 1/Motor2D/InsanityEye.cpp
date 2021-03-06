#include "InsanityEye.h"
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
#include "Prefabs.h"
#include "p2Defs.h"

InsanityEye::InsanityEye(pugi::xml_node& config) : Spell(insanity_eye, "insanity eye")
{
	prefab = new Prefab(App->scene->dummy_scene->test_boss->prefab->GetPosition().x, App->scene->dummy_scene->test_boss->prefab->GetPosition().y, "", NULLRECT);

	prefab->LoadAnimations(config);
	curr_anim = prefab->FindAnimation(Idle);

	draw_offset.x = config.child("drawoffset").attribute("x").as_int(0);
	draw_offset.y = config.child("drawoffset").attribute("y").as_int(0);
	size.x = config.child("size").attribute("w").as_int(10);
	size.y = config.child("size").attribute("h").as_int(10);

	insanity_speed -= App->scene->dummy_scene->round;
}

InsanityEye::~InsanityEye()
{
}

void InsanityEye::Start()
{
	prefab->CreateCollision(size.x, size.y, WORLD, PLAYER);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);

	if (prefab->pbody != nullptr) 
	{
		iPoint vel;
		int player_x, player_y;
		App->player->player->pbody->GetPosition(player_x, player_y);
		int boss_x, boss_y;
		for (p2List_item<Boss*>* boss = App->enemies->enemies.start; boss != nullptr; boss = boss->next) 
		{
			if (boss->data->shoot) 
			{
				boss->data->prefab->pbody->GetPosition(boss_x, boss_y);
				break;
			}
		}
		float delta_x = -player_x + boss_x - 400;
		float increment = 0;
		if (player_y < 355) increment += 40;
		float delta_y = -player_y + boss_y - increment;
		float alpha = atan(delta_y / delta_x);

		vel.x = (insanity_speed + App->scene->dummy_scene->round)*cos(alpha);
		vel.y = (insanity_speed + App->scene->dummy_scene->round)*sin(alpha);

		prefab->pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
	}
}

bool InsanityEye::Update(float dt)
{
	//LOG("%f", dt);

	if (collided)
	{
		if (prefab->pbody->body->GetLinearVelocity().x != 0 || prefab->pbody->body->GetLinearVelocity().y != 0)
			prefab->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

		if (prefab->animations[curr_anim]->Finished()) 
		{
			to_delete = true;
		}
	}
	else if(PlayerTouched())
	{
		prefab->pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		return_boss = true;
	}

	if(return_boss)
	{
		prefab->pbody->body->SetTransform(b2Vec2(prefab->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(600*dt)), prefab->pbody->body->GetPosition().y), 0);

		if (App->scene->dummy_scene->test_boss->prefab->pbody->body->GetPosition().y > prefab->pbody->body->GetPosition().y)
		{
			prefab->pbody->body->SetTransform(b2Vec2(prefab->pbody->body->GetPosition().x, prefab->pbody->body->GetPosition().y + PIXEL_TO_METERS((int)(200*dt))), 0);
		}
		else if (App->scene->dummy_scene->test_boss->prefab->pbody->body->GetPosition().y < prefab->pbody->body->GetPosition().y)
		{
			prefab->pbody->body->SetTransform(b2Vec2(prefab->pbody->body->GetPosition().x, prefab->pbody->body->GetPosition().y - PIXEL_TO_METERS((int)(200*dt))), 0);
		}

	
		if ((int)App->scene->dummy_scene->test_boss->prefab->pbody->body->GetPosition().x < (int)prefab->pbody->body->GetPosition().x && !dead)
		{
			to_delete = true;
		}
	}
	

	return true;
}

void InsanityEye::Draw()
{
	if(!dead)
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x + draw_offset.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[curr_anim]->GetCurrentFrameRect());
}

bool InsanityEye::PlayerTouched()
{
	//LOG("%d > %d", (int)App->player->player->pbody->body->GetPosition().x, (int)prefab->pbody->body->GetPosition().x);
	if ((int)App->player->player->pbody->body->GetPosition().x > (int)prefab->pbody->body->GetPosition().x)
	{
		return true;
	}
	return false;
}

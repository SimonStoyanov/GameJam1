#include "Hairball.h"
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

Hairball::Hairball(pugi::xml_node & config) : Spell(hairball, "hairball")
{
	PhysBody* player_body = App->player->player->pbody;
	int x, y;
	player_body->GetPosition(x, y);

	prefab = new Prefab(x + player_body->width, y + player_body->height, "", NULLRECT);

	prefab->LoadAnimations(config);
	curr_anim = prefab->FindAnimation(Idle);

	draw_offset.x = config.child("drawoffset").attribute("x").as_int(0);
	draw_offset.y = config.child("drawoffset").attribute("y").as_int(0);
	size.x = config.child("size").attribute("w").as_int(10);
	size.y = config.child("size").attribute("h").as_int(10);
}

Hairball::~Hairball()
{
}

void Hairball::Start()
{
	prefab->CreateCollision(size.x, size.y, PLAYER, WORLD);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);
	prefab->pbody->body->SetLinearVelocity(b2Vec2(15, 0));
	prefab->pbody->body->SetBullet(true);
}

bool Hairball::Update()
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

void Hairball::Draw()
{
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x + draw_offset.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[curr_anim]->GetCurrentFrameRect());
}

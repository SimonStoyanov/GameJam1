#include "Shield.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"

Shield::Shield(pugi::xml_node & config) : Spell(shield, "shield")
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

Shield::~Shield()
{
}

void Shield::Start()
{
	prefab->CreateCollision(size.x, size.y, PLAYER, WORLD);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);
}

bool Shield::Update()
{
	if (collided) {
		if (prefab->animations[curr_anim]->Finished()) {
			to_delete = true;
		}
	}
	return true;
}

void Shield::Draw()
{
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x + draw_offset.x + App->render->camera.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[curr_anim]->GetCurrentFrameRect());
}

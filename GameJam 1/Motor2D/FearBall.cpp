#include "fearball.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"

Fearball::Fearball(pugi::xml_node& config) : Spell(fearball, "fearball")
{
	PhysBody* player_body = App->player->player->pbody;
	int x, y;
	player_body->GetPosition(x, y);

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	float delta_x = mouse.x - App->render->camera.x - x - player_body->width;
	float delta_y = mouse.y - App->render->camera.y - y - player_body->height;

	prefab = new Prefab(x + player_body->width, y + player_body->height, "", NULLRECT);

	prefab->LoadAnimations(config);
	curr_anim = prefab->FindAnimation(Idle);

	alpha = atan(delta_y / delta_x);

	if (delta_x < 0)
	{
		vel.x = fearball_speed*-cos(alpha);
		vel.y = fearball_speed*-sin(alpha);
		alpha += PI;
	}
	else
	{
		vel.x = fearball_speed*cos(alpha);
		vel.y = fearball_speed*sin(alpha);
	}

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
	prefab->CreateCollision(size.x, size.y, PLAYER, WORLD);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);
	if (prefab->pbody != nullptr)
		prefab->pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
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
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x + draw_offset.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[curr_anim]->GetCurrentFrameRect(), 1.0f, RADTODEG*alpha);
}
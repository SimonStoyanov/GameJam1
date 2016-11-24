#include "Fireball.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"

Fireball::Fireball(pugi::xml_node& config) : Spell(fireball, "fireball")
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

	float alpha = atan(delta_y / delta_x);

	if (delta_x < 0)
	{
		vel.x = fireball_speed*-cos(alpha);
		vel.y = fireball_speed*-sin(alpha);
	}
	else
	{
		vel.x = fireball_speed*cos(alpha);
		vel.y = fireball_speed*sin(alpha);
	}

	LOG("player %d %d", x, y);
}

Fireball::~Fireball()
{
}

void Fireball::Start()
{
	prefab->CreateCollision(15,15, PLAYER, WORLD);
	prefab->pbody->listener = App->spellmanager;
	prefab->pbody->body->SetGravityScale(0);
	if (prefab->pbody != nullptr)
		prefab->pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

bool Fireball::Update()
{
	return true;
}

void Fireball::Draw()
{
	App->render->Blit(App->spellmanager->GetAtlas(), prefab->GetPosition().x, prefab->GetPosition().y, &prefab->animations[curr_anim]->GetCurrentFrameRect());
}

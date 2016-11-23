#include "Fireball.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"

Fireball::Fireball() : Spell(fireball, "fireball")
{
	PhysBody* player_body = App->player->player->pbody;
	int x, y;
	player_body->GetPosition(x, y);

	iPoint xy;
	App->input->GetMousePosition(xy.x, xy.y);

	float delta_x = xy.x - App->render->camera.x - x - player_body->width;
	float delta_y = xy.y - App->render->camera.y - y - player_body->height;


	prefab = new Prefab(x + App->render->camera.x + player_body->width, y + App->render->camera.y + player_body->height, "", NULLRECT);
	prefab->CreateCollision(10, PLAYER, WORLD);
	prefab->pbody->listener = App->spellmanager;

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

bool Fireball::Update()
{
	if (prefab->pbody != nullptr)
		prefab->pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

	return true;
}

void Fireball::Draw()
{
}

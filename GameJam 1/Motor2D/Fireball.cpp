#include "Fireball.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>

Fireball::Fireball() : Spell(fireball, "fireball")
{
	int x, y;
	App->player->player->pbody->GetPosition(x, y);

	iPoint xy;
	App->input->GetMousePosition(xy.x, xy.y);

	float delta_x = xy.x - App->render->camera.x - x - 4;
	float delta_y = xy.y - App->render->camera.y - y - 10;


	prefab = Prefab(x + App->render->camera.x, y + App->render->camera.y + 16, "", NULLRECT);
	prefab.CreateCollision(10, PLAYER, BOSS);

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
	if (prefab.pbody != nullptr)
		prefab.pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

	return true;
}

void Fireball::Draw()
{
}

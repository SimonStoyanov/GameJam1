#include "Fireball.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"

Fireball::Fireball() : Spell(fireball, "fireball")
{
	int x, y;
	App->player->player->pbody->GetPosition(x, y);
	prefab = Prefab(x + App->render->camera.x, y + App->render->camera.y, NULL, NULLRECT);
	prefab.CreateCollision(10, PLAYER, BOSS);
	LOG("player %d %d", x, y);
}

Fireball::~Fireball()
{
}

void Fireball::Draw()
{

}

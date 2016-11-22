#include "Spell.h"
#include "Player.h"
#include "j1App.h"

Spell::Spell()
{
}

Spell::Spell(Spelltypes type_) : type(type_)
{
}

Spell::Spell(Spelltypes type_, char* name_) : type(type_), name(name_)
{
	int x, y;
	App->player->player->pbody->GetPosition(x, y);
	prefab = Prefab(x, y, NULL, NULLRECT);
	prefab.CreateCollision(1, PLAYER, BOSS);
}

Spell::~Spell()
{
}

void Spell::Start()
{
}

bool Spell::Update()
{
	return true;
}

void Spell::Draw()
{
}

void Spell::Cleanup()
{
}

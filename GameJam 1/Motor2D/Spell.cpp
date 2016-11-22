#include "Spell.h"

Spell::Spell()
{
}

Spell::Spell(Spelltypes type_) : type(type_)
{
}

Spell::Spell(Spelltypes type_, char* name_) : type(type_), name(name_)
{
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

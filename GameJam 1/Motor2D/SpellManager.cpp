#include "SpellManager.h"
#include "j1App.h"

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
}

bool SpellManager::Awake(pugi::xml_node & node)
{
	return true;
}

bool SpellManager::Start()
{
	return true;
}

bool SpellManager::PreUpdate()
{
	return true;
}

bool SpellManager::Update(float dt)
{
	return true;
}

bool SpellManager::PostUpdate()
{
	return true;
}

bool SpellManager::CleanUp()
{
	while (spells.count() > 0) {
		spells.del(spells.end);
	}
	return true;
}

Spell* SpellManager::CreateSpell(Spelltypes type, char* name)
{
	Spell* spell = new Spell(type, name);
	spells.add(spell);
	return spell;
}

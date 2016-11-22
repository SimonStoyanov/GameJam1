#include "SpellManager.h"
#include "j1App.h"
#include "Fireball.h"

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
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		spell_item->data->Update();
		spell_item->data->Draw();
		spell_item = spell_item->next;
	}
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

Spell* SpellManager::CreateSpell(Spelltypes type)
{
	Spell* spell = nullptr;
	switch (type)
	{
	case fireball:
		spell = new Fireball();
		break;
	default:
		break;
	}
	if (spell != nullptr)
		spells.add(spell);

	return spell;
}

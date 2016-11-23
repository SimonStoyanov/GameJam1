#include "SpellManager.h"
#include "j1App.h"
#include "Fireball.h"
#include "j1Input.h"
#include "j1Text.h"

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
	time = new j1Timer();

	return true;
}

bool SpellManager::PreUpdate()
{
	return true;
}

bool SpellManager::Update(float dt)
{

	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) 
	{
		spell_item->data->Update();
		spell_item->data->Draw();
		spell_item = spell_item->next;
	}


	// Q ---------------------

	if (time->ReadSec() > timeQ + GetCd(App->spellmanager->Q))
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			App->spellmanager->CreateSpell(App->spellmanager->Q);
			timeQ = time->ReadSec();
		}
	}
	if((timeQ + GetCd(App->spellmanager->Q) - time->ReadSec()) > 0)
	{
		p2SString tmp; tmp.create("Q: %0.1f", timeQ + GetCd(App->spellmanager->Q) - time->ReadSec());
		App->text->cdQ->SetText(tmp);
	}

	// -----------------------

	// W ---------------------

	// -----------------------

	// E ---------------------

	// -----------------------

	// R ---------------------

	// -----------------------

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

void SpellManager::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (IsSpell(bodyA)) {
		if (bodyB->body->GetFixtureList()->GetFilterData().categoryBits == WORLD) {
			DeleteSpell(bodyA);
		}
	}
}

bool SpellManager::IsSpell(PhysBody * body)
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		if (spell_item->data->prefab.pbody == body)
			return true;
		spell_item = spell_item->next;
	}
	return false;
}

void SpellManager::DeleteSpell(PhysBody * body)
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		if (spell_item->data->prefab.pbody == body) {
			spells.del(spell_item);
			break;
		}
		spell_item = spell_item->next;
	}
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

int SpellManager::GetCd(Spelltypes type)
{
	switch (type)
	{
	case fireball:
		return 2;
		break;
	}
}

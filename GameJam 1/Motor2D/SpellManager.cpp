#include "SpellManager.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Input.h"
#include "j1Text.h"
#include "j1Textures.h"
#include "ModuleEnemies.h"
#include "Fireball.h"
#include "JumpAttack.h"
#include "FearBall.h"
#include "InsanityEye.h"
#include "j1Render.h"
#include "Player.h"

#define EMPTY -1

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
	char* buf;
	int size = App->fs->Load("SpellsConfig.xml", &buf);
	spellconfig_doc.load_buffer(buf, size);
	RELEASE(buf);
	spells_config = spellconfig_doc.child("config");

	p2SString path(spells_config.child("spritesheet").attribute("path").as_string(nullptr));

	spells_atlas = App->tex->Load(path.GetString());

	// Cooldowns
	time = new j1Timer();
	timeQ = -99;
	timeW = -99;
	timeE = -99;
	timeR = -99;

	return true;
}

bool SpellManager::PreUpdate()
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		if (spell_item->data->to_delete) {
			App->physics->DeleteObject(spell_item->data->prefab->pbody);
			spells.del(spell_item);
		}
		spell_item = spell_item->next;
	}
	return true;
}

bool SpellManager::Update(float dt)
{
	if (!App->player->active)
		return true;
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) 
	{
		spell_item->data->Update();
		spell_item = spell_item->next;
	}

	// Spells
	// Q ---------------------

	if (time->ReadSec() > timeQ + GetCd(Q) && GetCd(Q) != EMPTY)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			CreateSpell(Q);
			timeQ = time->ReadSec();
		}
	}
	
	p2SString tmpQ;
	if ((timeQ + GetCd(Q) - time->ReadSec()) > 0)
	{
		tmpQ.create("%0.1f", timeQ + GetCd(Q) - time->ReadSec());
		Qcd = true;
	}
	else
	{
		tmpQ.create(" 0", timeQ + GetCd(Q) - time->ReadSec());
		Qcd = false;
	}
	App->text->cdQ->SetText(tmpQ);
	// -----------------------

	// W ---------------------

	if (time->ReadSec() > timeW + GetCd(W) && GetCd(W) != EMPTY)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			CreateSpell(W);
			timeW = time->ReadSec();
		}
	}

	p2SString tmpW;
	if ((timeW + GetCd(W) - time->ReadSec()) > 0)
	{
		tmpW.create("%0.1f", timeW + GetCd(W) - time->ReadSec());
		Wcd = true;
	}
	else
	{
		tmpW.create(" 0", timeW + GetCd(W) - time->ReadSec());
		Wcd = false;
	}
	App->text->cdW->SetText(tmpW);

	// -----------------------

	// E ---------------------

	if (time->ReadSec() > timeE + GetCd(E) && GetCd(E) != EMPTY)
	{
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			CreateSpell(E);
			timeE = time->ReadSec();
		}
	}

	p2SString tmpE;
	if ((timeE + GetCd(E) - time->ReadSec()) > 0)
	{
		tmpE.create("%0.1f", timeE + GetCd(E) - time->ReadSec());
		Ecd = true;
	}
	else
	{
		tmpE.create(" 0", timeE + GetCd(E) - time->ReadSec());
		Ecd = false;
	}
	App->text->cdE->SetText(tmpE);

	// -----------------------

	// R ---------------------

	if (time->ReadSec() > timeR + GetCd(R) && GetCd(R) != EMPTY)
	{
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			CreateSpell(R);
			timeE = time->ReadSec();
		}
	}

	p2SString tmpR;
	if ((timeE + GetCd(R) - time->ReadSec()) > 0)
	{
		tmpR.create("%0.1f", timeE + GetCd(R) - time->ReadSec());
		Rcd = true;
	}
	else
	{
		tmpR.create(" 0", timeE + GetCd(R) - time->ReadSec());
		Rcd = false;
	}
	App->text->cdR->SetText(tmpR);

	// -----------------------

	return true;
}

bool SpellManager::PostUpdate()
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr)
	{
		spell_item->data->Draw();
		spell_item = spell_item->next;
	}
	return true;
}

bool SpellManager::CleanUp()
{
	while (spells.count() > 0) {
		App->physics->DeleteObject(spells.end->data->prefab->pbody);
		spells.del(spells.end);
	}
	return true;
}

void SpellManager::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (IsSpell(bodyA)) {
		if (bodyB->body->GetFixtureList()->GetFilterData().categoryBits == WORLD) {
			for (p2List_item<Boss*>* boss_item = App->enemies->enemies.start; boss_item != nullptr; boss_item = boss_item->next) {
				if (boss_item->data->prefab->pbody == bodyB && boss_item->data->curr_hp > 0) {
					boss_item->data->curr_hp -= 1;
				}
			}
			DeleteSpell(bodyA);
		}
		if (bodyB->body->GetFixtureList()->GetFilterData().categoryBits == PLAYER) {
			App->player->curr_hp -= 1;
			DeleteSpell(bodyA);
		}
	}
}

bool SpellManager::IsSpell(PhysBody * body)
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		if (spell_item->data->prefab->pbody == body)
			return true;
		spell_item = spell_item->next;
	}
	return false;
}

void SpellManager::DeleteSpell(PhysBody * body)
{
	p2List_item<Spell*>* spell_item = spells.start;
	while (spell_item != nullptr) {
		if (spell_item->data->prefab->pbody == body) {
			spell_item->data->collided = true;
			spell_item->data->curr_anim = spell_item->data->prefab->FindAnimation(Explode);
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
		spell = new Fireball(spells_config.child("fireball"));
		spell->SetDamage(1);
		spell->Start();
		break;
	case jump_attack:
		spell = new JumpAttack(spells_config.child("jump_attack"));
		spell->SetDamage(1);
		spell->Start();
		break;
	case fearball:
		spell = new Fearball(spells_config.child("fearball"));
		spell->SetDamage(1);
		spell->Start();
		break;
	case insanity_eye:
		spell = new InsanityEye(spells_config.child("insanityeye"));
		spell->SetDamage(1);
		spell->Start();
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
		return 1;
		break;
	case jump_attack:
		return 2;
		break;
	case unknown:
		return EMPTY;
	default:
		return -99;
	}
}

SDL_Texture * SpellManager::GetAtlas() const
{
	return spells_atlas;
}

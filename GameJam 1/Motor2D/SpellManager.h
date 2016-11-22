#ifndef _SPELLMANAGER_
#define _SPELLMANAGER_

#include "j1Module.h"
#include "p2List.h"
#include "Spell.h"

class SpellManager : public j1Module {
public:
	p2List<Spell*> spells;

public:

	SpellManager();

	~SpellManager();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Spell* CreateSpell(Spelltypes type, char* name);

private:

private:


};

#endif
#ifndef _SPELLMANAGER_
#define _SPELLMANAGER_

#include "j1Module.h"
#include "p2List.h"

class Spell;

class SpellManager : public j1Module {
public:
	p2List<Spell*> spells;

public:
	bool Update(float dt);

private:

private:


};

#endif
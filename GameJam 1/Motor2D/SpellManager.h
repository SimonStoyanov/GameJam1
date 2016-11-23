#ifndef _SPELLMANAGER_
#define _SPELLMANAGER_

#include "j1Module.h"
#include "p2List.h"
#include "Spell.h"
#include "j1Timer.h"

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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	bool IsSpell(PhysBody* body);

	void DeleteSpell(PhysBody* body);

	Spell* CreateSpell(Spelltypes type);
	int GetCd(Spelltypes type);


	j1Timer* time;

	Spelltypes Q;
	float timeQ;
	Spelltypes W;
	float timeW;
	Spelltypes E;
	float timeE;
	Spelltypes R;
	float timeR;

private:

private:


};

#endif
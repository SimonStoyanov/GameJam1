#ifndef _SPELLMANAGER_
#define _SPELLMANAGER_

#include "j1Module.h"
#include "p2List.h"
#include "Spell.h"
#include "j1Timer.h"

struct SDL_Texture;

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

	Spell* GetSpell(PhysBody* body) const;

	void DeleteSpell(PhysBody* body);

	Spell* CreateSpell(Spelltypes type);
	int GetCd(Spelltypes type);

	SDL_Texture* GetAtlas()const;

	j1Timer* time;

	Spelltypes Q;
	float timeQ;
	bool Qcd = false;
	Spelltypes W;
	float timeW;
	bool Wcd = false;
	Spelltypes E;
	float timeE;
	bool Ecd = false;
	Spelltypes R;
	float timeR;
	bool Rcd = false;


private:
	pugi::xml_document spellconfig_doc;
	pugi::xml_node spells_config;
	SDL_Texture* spells_atlas;

private:


};

#endif
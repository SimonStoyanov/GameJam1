#ifndef Spells_
#define Spells_

#include "p2SString.h"
#include "Prefabs.h"

class Spell {
private:
	p2SString name;
	Prefab* prefab;
	uint damage;
	enum SpellTypes {
		ranged,
		melee,
		unknown
	} type;
public:
	Spell();
	Spell(SpellTypes type_);
	Spell(SpellTypes type_, uint damage_);
	~Spell();

	virtual void Start();
	virtual bool Update(float dt);
	virtual void Draw();
	virtual void Cleanup();
};


#endif // !PlayerSpells_


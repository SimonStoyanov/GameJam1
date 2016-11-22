#ifndef Spells_
#define Spells_

#include "p2SString.h"
#include "Prefabs.h"

enum Spelltypes {
	ranged,
	melee,
	unknown
};

class Spell{
private:
	p2SString name;
	Spelltypes type;
	Prefab* prefab;
public:
	Spell();
	Spell(Spelltypes type_);
	Spell(Spelltypes type_, char* name);
	~Spell();

	virtual void Start();
	virtual bool Update(float dt);
	virtual void Draw();
	virtual void Cleanup();
};


#endif // !PlayerSpells_


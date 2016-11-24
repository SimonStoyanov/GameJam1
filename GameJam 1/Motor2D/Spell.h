#ifndef Spells_
#define Spells_

#include "p2SString.h"
#include "Prefabs.h"

enum Spelltypes {
	fireball,
	unknown
};

class Spell{
public:
	p2SString name;
	Spelltypes type;
	Prefab* prefab;
	bool to_delete = false;
public:
	Spell();
	Spell(Spelltypes type_);
	Spell(Spelltypes type_, char* name);
	~Spell();

	virtual void Start();
	virtual bool Update();
	virtual void Draw();
	virtual void Cleanup();
	void SetDamage(int damage);
	int GetDamage() const;

private:
	int dmg;
};


#endif // !PlayerSpells_


#ifndef Spells_
#define Spells_

#include "p2SString.h"
#include "Prefabs.h"

#define PI 3.141592653589793

enum Spelltypes {
	fireball,
	jump_attack,
	fearball,
	insanity_eye,
	firebarrage,
	unknown
};

class Spell{
public:
	p2SString name;
	Spelltypes type;
	Prefab* prefab;
	bool to_delete = false;
	bool collided = false;
	int curr_anim;
	iPoint size;
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


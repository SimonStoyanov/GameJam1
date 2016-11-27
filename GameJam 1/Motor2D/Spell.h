#ifndef Spells_
#define Spells_

#include "p2SString.h"
#include "Prefabs.h"

#define PI 3.141592653589793

enum Spelltypes {
	fireball,
	shield,
	jump_attack,
	fearball,
	insanity_eye,
	firebarrage,
	shapeball,
	ghost,
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
	void SetTime(float time_);
	float GetTime() const;

private:
	int dmg;
	float time;
};


#endif // !PlayerSpells_


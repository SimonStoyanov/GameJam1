#ifndef Fireball_
#define Fireball_

#include "Spell.h"

class Fireball : public Spell {
private:
	uint damage;
public:
	Fireball();
	~Fireball();

	void Draw();
};

#endif // !Fireball_

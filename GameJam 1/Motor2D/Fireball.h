#ifndef Fireball_
#define Fireball_

#include "Spell.h"

class Fireball : public Spell {
private:
	uint damage;
	fPoint vel;

	float fireball_speed = 10;
public:
	Fireball();
	~Fireball();

	bool Update();
	void Draw();
};

#endif // !Fireball_

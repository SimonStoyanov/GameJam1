#ifndef Fireball_
#define Fireball_

#include "Spell.h"

class Fireball : public Spell {
private:
	fPoint vel;

	float fireball_speed = 10;
public:
	Fireball(pugi::xml_node& config);
	~Fireball();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Fireball_

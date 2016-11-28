#ifndef Fireball_
#define Fireball_

#include "Spell.h"

class Fireball : public Spell {
private:
	fPoint vel;
	float alpha;
	float fireball_speed = 10;
	iPoint draw_offset;
public:
	Fireball(pugi::xml_node& config);
	~Fireball();

	void Start();
	bool Update(float dt);
	void Draw();
};

#endif // !Fireball_

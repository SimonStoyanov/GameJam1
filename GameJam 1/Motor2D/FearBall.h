#ifndef _FEARBALL_
#define _FEARBALL_

#include "Spell.h"

class Fearball : public Spell {
private:
	fPoint vel;
	float alpha;
	float fearball_speed = 10;
	iPoint draw_offset;
public:
	Fearball(pugi::xml_node& config);
	~Fearball();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Fireball_


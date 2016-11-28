#ifndef _SHAPEBALL_
#define _SHAPEBALL_

#include "Spell.h"

class Shapeball : public Spell {
private:
	float fearball_speed = -5;
	iPoint draw_offset;
public:
	Shapeball(pugi::xml_node& config);
	~Shapeball();

	void Start();
	bool Update(float dt);
	void Draw();
};

#endif // !Shapeball_



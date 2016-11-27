#ifndef _HAIRBALL_
#define _HAIRBALL_

#include "Spell.h"

class Hairball : public Spell {
private:
	float fearball_speed = -5;
	iPoint draw_offset;
public:
	Hairball(pugi::xml_node& config);
	~Hairball();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Shapeball_


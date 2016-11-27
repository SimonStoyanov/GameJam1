#ifndef _SHIELD_
#define _SHIELD_

#include "Spell.h"

class j1Timer;

class Shield : public Spell {
private:
	iPoint draw_offset;
	j1Timer* timer;
public:
	Shield(pugi::xml_node& config);
	~Shield();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Fireball_
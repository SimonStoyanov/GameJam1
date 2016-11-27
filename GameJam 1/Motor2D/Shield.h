#ifndef _SHIELD_
#define _SHIELD_

#include "Spell.h"

class Shield : public Spell {
private:
	iPoint draw_offset;
public:
	Shield(pugi::xml_node& config);
	~Shield();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Fireball_
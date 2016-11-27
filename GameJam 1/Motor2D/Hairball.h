#ifndef _HAIRBALL_
#define _HAIRBALL_

#include "Spell.h"

class Hairball : public Spell {
private:
	iPoint draw_offset;
public:
	Hairball(pugi::xml_node& config);
	Hairball(pugi::xml_node& config, int off_x, int off_y);
	~Hairball();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Shapeball_


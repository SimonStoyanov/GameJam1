#ifndef _INSANITYEYE_
#define _INSANITYEYE_

#include "Spell.h"

class InsanityEye : public Spell {
private:
	float insanity_speed = -5;
	iPoint draw_offset;
public:
	InsanityEye(pugi::xml_node& config);
	~InsanityEye();

	void Start();
	bool Update();
	void Draw();
};

#endif // !Fireball_
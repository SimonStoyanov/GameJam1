#ifndef _JUMPATTACK_
#define _JUMPATTACK_

#include "Spell.h"

class JumpAttack : public Spell {
public:
	JumpAttack(pugi::xml_node& config);
	~JumpAttack();

	void Start();
	bool Update(float dt);
	void Draw();
};

#endif // !_JUMPATTACK_

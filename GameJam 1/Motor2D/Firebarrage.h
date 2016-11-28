#ifndef _BARRAGE_
#define _BARRAGE_

#include "Spell.h"


class Firebarrage : public Spell 
{
private:
	fPoint vel;
	float alpha;
	float fireball_speed = 10;
	iPoint draw_offset;
	Prefab* prefab2;
	Prefab* prefab3;

	int move_x;
	int move_y;
public:
	Firebarrage(pugi::xml_node& config, int x, int y);
	~Firebarrage();

	void Start();
	bool Update(float dt);
	void Draw();
};

#endif // !Fireball_
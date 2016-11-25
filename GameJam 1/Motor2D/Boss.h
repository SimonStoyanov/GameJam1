#ifndef _BOSS_
#define _BOSS_

#include "Prefabs.h"
#include "j1App.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"


class Animation;

enum EnemiesTypes {
	fear, unknow,
};

class Boss {
public:
	Boss(){}
	Boss(EnemiesTypes type) : type(type) 
	{	
		max_hp = curr_hp = 10 + 2 * App->scene->dummy_scene->round;
	}

	virtual void Start(){}
	virtual bool Update(float dt) { return true; }
	virtual void Draw(){}
	virtual void Shoot(){}

public:
	Prefab* prefab;
	EnemiesTypes type;
	int max_hp;
	int curr_hp;
	int current_anim;
	iPoint draw_offset;
private:
	
private:

};

#endif // !_BOSS_
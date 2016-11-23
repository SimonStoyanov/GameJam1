#ifndef _BOSS_
#define _BOSS_

#include "Prefabs.h"
#include "j1App.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"
#include "PugiXml\src\pugixml.hpp"

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

	virtual bool Update() { return true; }
	virtual void Draw(){}

	void LoadAnimations(pugi::xml_node& config) {
		for (pugi::xml_node anim = config.child("anim"); anim != NULL; anim = anim.next_sibling("anim")) {
			p2List<SDL_Rect> anim_rects;
			float speed = anim.attribute("speed").as_float(1.0f);
			int type = anim.attribute("type").as_int(-1);
			for (pugi::xml_node frame = anim.child("frame"); frame != NULL; frame = frame.next_sibling("frame")) {
				SDL_Rect new_frame = { frame.attribute("x").as_int(0),frame.attribute("y").as_int(0),frame.attribute("w").as_int(0),frame.attribute("h").as_int(0) };
				anim_rects.add(new_frame);
			}
			if(type != -1)
				prefab.animations.add(new Animation(anim_rects, speed, Idle));
		}
	}
public:
	Prefab prefab;
	EnemiesTypes type;
	int max_hp;
	int curr_hp;
	int current_anim;

private:
	
private:

};

#endif // !_BOSS_
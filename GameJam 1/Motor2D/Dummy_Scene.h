#ifndef _DUMMY_
#define _DUMMY_

#include "Scene.h"
#include "p2List.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Textures.h"
#include "SDL\include\SDL.h"
#include "Prefabs.h"


class Boss;
class PhysBody;
class RandomGenerator;
class Text;

class Dummy : public Scene
{

public:
	Dummy();
	~Dummy();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void Draw();
	bool CleanUp();

	bool Dummy::IsGroundBody(PhysBody* body);

public:
	int round = 1; //round counter (each win adds 1)
	RandomGenerator* platforms_rand;

private:
	p2List<Prefab*> grounds;
	Prefab* platform;

	Sprite background;
	Sprite forward;
	Sprite far_background;
	float back_speed;
	float for_speed;
	float farback_speed;

	uint forward_min = 0;
	uint background_min = 0;
	uint farback_min = 0;

	SDL_Rect ground_rect;
	SDL_Rect back_rect;
	SDL_Rect forward_rect;
	SDL_Rect farback_rect;

	int forward_count = 1;
	int background_count = 1;
	int farback_count = 1;

	p2SString parallax_spritesheet;

	Boss* test_boss;
	Text* boss_hp;
};

#endif
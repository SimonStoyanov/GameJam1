#ifndef _DUMMY_
#define _DUMMY_

#include "Scene.h"
#include "p2List.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Textures.h"
#include "SDL\include\SDL.h"
#include "Prefabs.h"

class FearBoss;
class PhysBody;
class RandomGenerator;

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
private:
	p2List<Prefab*> grounds;
	RandomGenerator* test_rand;
	Prefab* test_pref;

	Sprite background;
	Sprite forward;
	float back_speed;
	float for_speed;

	uint forward_min = 0;
	uint background_min = 0;

	SDL_Rect ground_rect;
	SDL_Rect back_rect;
	SDL_Rect forward_rect;

	int forward_count = 1;
	int background_count = 1;

	p2SString parallax_spritesheet;

	FearBoss* test_boss;

};

#endif
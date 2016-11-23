#include "Scene.h"
#include "p2List.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Textures.h"
#include "SDL\include\SDL.h"
#include "Prefabs.h"

class PhysBody;
class RandomGenerator;

class Dummy : public Scene
{

public:
	Dummy();
	~Dummy();

	bool Start();
	bool Update(float dt);
	void Draw();
	bool CleanUp();

	bool Dummy::IsGroundBody(PhysBody* body);

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
	SDL_Rect ford_rect;

	int count = 1;

	p2SString parallax_spritesheet;

};
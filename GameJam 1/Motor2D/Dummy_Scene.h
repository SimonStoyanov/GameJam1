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
	Sprite path_image;

	uint path_min = 0;
	uint background_min = 0;
};
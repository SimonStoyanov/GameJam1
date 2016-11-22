#include "Scene.h"
#include "p2List.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Textures.h"

class Prefab;
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

	SDL_Texture* background_tex;

};
#include "Scene.h"
#include "p2List.h"

class Prefab;
class PhysBody;

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
};
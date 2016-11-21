#include "Scene.h"

class Prefab;

class Dummy : public Scene
{

public:
	Dummy();
	~Dummy();

	bool Start();
	bool Update(float dt);
	void Draw();
	bool CleanUp();

private:
	Prefab* ground = nullptr;
};
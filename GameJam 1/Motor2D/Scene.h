#ifndef _SCENE_
#define _SCENE_

class Scene 
{
public:
	Scene()
	{}

	virtual ~Scene(){}

	virtual bool Start() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual void Draw() {}
	virtual bool CleanUp() { return true; }

public:

private:
};

#endif

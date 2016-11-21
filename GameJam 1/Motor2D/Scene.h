#ifndef _SCENE_
#define _SCENE_

class Scene {
public:

public:
	Scene(){}
	~Scene(){}

	virtual bool Update(float dt) { return true; }
	virtual void Draw() {	}

private:

private:
};

#endif

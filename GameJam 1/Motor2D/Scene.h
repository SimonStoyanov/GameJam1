#ifndef _SCENE_
#define _SCENE_

class Scene {
public:

public:
	Scene(bool _enabled = true) : enabled(_enabled)
	{}
	~Scene(){}

	virtual bool Start() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual void Draw() {}
	virtual bool CleanUp() { return true; }
	
	bool IsEnabled()
	{
		if (enabled)
			return true;
		return false;
	}
	void Enable()
	{
		enabled = true;
	}
	void Disable()
	{
		CleanUp();
		enabled = false;
	}

private:
private:
	bool enabled;
};

#endif

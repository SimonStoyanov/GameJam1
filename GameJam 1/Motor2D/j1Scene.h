#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class Text;
class Scene;

class Dummy;
class IntroScene;
class WinScene;
class LoseScene;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void ChangeScene(Scene* new_scene);

public:
	Dummy* dummy_scene = nullptr;
	IntroScene* intro_scene = nullptr;
	WinScene* win_scene = nullptr;
	LoseScene* lose_scene = nullptr;

	bool have_fear = false;
	bool crazy = false;

private:
	Scene* current_scene = nullptr;
	int camera_speed;

};

#endif // __j1SCENE_H__
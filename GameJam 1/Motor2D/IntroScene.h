#ifndef _INTROSCENE_
#define _INTROSCENE_

#include "Scene.h"

class Button;

class IntroScene : public Scene {
public:
	IntroScene();

	~IntroScene();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void Draw();
	bool CleanUp();
public:

private:
	Button* play;

};

#endif
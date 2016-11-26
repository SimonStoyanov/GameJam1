#ifndef _INTROSCENE_
#define _INTROSCENE_

#include "Scene.h"

class Prefab;
class Button;
struct SDL_Texture;

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
	Button* play_button;
	Prefab* play;
	bool play_clicked = false;
	SDL_Texture* UI_tex;
	SDL_Texture* Background_tex;

};

#endif
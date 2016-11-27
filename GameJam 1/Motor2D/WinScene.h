#ifndef _WINSCENE_
#define _WINSCENE_

#include "Scene.h"

class Prefab;
class Button;
struct SDL_Texture;

class WinScene : public Scene {
public:
	WinScene();

	~WinScene();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void Draw();
	bool CleanUp();
public:

private:
	Button* next_button;
	Prefab* next;
	bool next_clicked = false;
	SDL_Texture* UI_tex;
	SDL_Texture* Background_tex;

};

#endif
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "SDL/include/SDL.h"
#include "j1App.h"

class Button
{
public:
	Button(int _x, int _y, int _w, int _h);
	~Button();

	bool MouseDown();
	bool MouseOver();

	void Draw();

public:
	bool		draw = true;

private:
	SDL_Rect	rect;
};

#endif // __BUTTON_H__
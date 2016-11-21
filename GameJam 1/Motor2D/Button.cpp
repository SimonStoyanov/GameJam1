// ----------------------------------------------------
// Button.cpp
// Basic button with click and mouse over functions
// ----------------------------------------------------

#include "Button.h"
#include "j1Input.h"
#include "j1Render.h"

Button::Button(int _x, int _y, int _w, int _h)
{
	rect.x = _x;
	rect.y = _y;
	rect.w = _w;
	rect.h = _h;

	draw = true;
}

Button::~Button()
{
}

bool Button::MouseDown()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x -= App->render->camera.x;
		mouse_y -= App->render->camera.y;

		if (mouse_x > rect.x && mouse_x < rect.x + rect.w)
		{
			if (mouse_y > rect.y && mouse_y < rect.y + rect.h)
			{
				return true;
			}
		}
	}
	return false;
}

bool Button::MouseOver()
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);
	mouse_x -= App->render->camera.x;
	mouse_y -= App->render->camera.y;

	if (mouse_x > rect.x && mouse_x < rect.x + rect.w)
	{
		if (mouse_y > rect.y && mouse_y < rect.y + rect.h)
		{
			return true;
		}
	}
	return false;
}

void Button::Draw()
{
	if(draw)
		App->render->DrawQuad(rect, 255, 255, 255, 255, false);
}

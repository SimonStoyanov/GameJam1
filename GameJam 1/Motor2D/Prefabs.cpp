// ----------------------------------------------------
// Prefabs.cpp
// Combolution of basic clases xD
// ----------------------------------------------------

#include "Prefabs.h"
#include "j1Textures.h"

Prefab::Prefab()
{
}

Prefab::Prefab(SDL_Texture * _texture, int x, int y, int w, int h, int pos_x, int pos_y)
{
	texture = _texture;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	pos.x = pos_x; pos.y = pos_y;
}

Prefab::Prefab(SDL_Texture* _texture, SDL_Rect _rect, int pos_x, int pos_y)
{
	texture = _texture;
	rect.x = _rect.x; rect.y = _rect.y; rect.w = _rect.w; rect.h = _rect.h;
	pos.x = pos_x; pos.y = pos_y;
}

Prefab::~Prefab()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}
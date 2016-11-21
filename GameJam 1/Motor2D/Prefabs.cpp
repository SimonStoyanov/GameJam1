// ----------------------------------------------------
// Prefabs.cpp
// Combolution of basic clases xD
// ----------------------------------------------------

#include "Prefabs.h"

Prefab::Prefab()
{
}

Prefab::Prefab(int pos_x, int pos_y, char * texture_path, SDL_Rect rect): sprite(Sprite(pos_x, pos_y, texture_path, rect))
{
}

Prefab::~Prefab()
{
	if (sprite.texture != nullptr)
	{
		App->tex->UnLoad(sprite.texture);
		sprite.texture = nullptr;
	}
}
// ----------------------------------------------------
// Prefabs.cpp
// Combolution of basic clases xD
// ----------------------------------------------------

#include "Prefabs.h"

Prefab::Prefab()
{
}

Prefab::Prefab(int pos_x, int pos_y, char * texture_path, SDL_Rect rect) : sprite(Sprite(pos_x, pos_y, texture_path, rect))
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

void Prefab::CreateCollision(int width, int height, int cat, int mask)
{
	body = App->physics->CreateRectangle(sprite.pos.x, sprite.pos.y, width, height, 0.0f, cat, mask);
}

void Prefab::CreateCollision(int radius, int cat, int mask)
{
	body = App->physics->CreateCircle(sprite.pos.x, sprite.pos.y, radius, 0.0f, cat, mask);
}

void Prefab::CreateStaticCollision(int width, int height, int cat, int mask)
{
	body = App->physics->CreateStaticRectangle(sprite.pos.x, sprite.pos.y, width, height, 0.0f, cat, mask);
}

void Prefab::CreateStaticCollision(int radius, int cat, int mask)
{
	body = App->physics->CreateStaticCircle(sprite.pos.x, sprite.pos.y, radius, 0.0f, cat, mask);
}


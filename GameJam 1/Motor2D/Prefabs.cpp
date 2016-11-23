// ----------------------------------------------------
// Prefabs.cpp
// Combolution of basic clases xD
// ----------------------------------------------------

#include "Prefabs.h"

Prefab::Prefab()
{
}

Prefab::Prefab(int pos_x, int pos_y, const char * texture_path, SDL_Rect rect) : sprite(Sprite(pos_x, pos_y, texture_path, rect))
{
}

Prefab::Prefab(int pos_x, int pos_y, SDL_Texture * texture, SDL_Rect rect) : sprite(Sprite(pos_x, pos_y, texture, rect))
{
}

Prefab::Prefab(Prefab* prefab)
{
	pbody = prefab->pbody;
	sprite.pos = prefab->sprite.pos;
	sprite.rect = prefab->sprite.rect;
	sprite.texture = prefab->sprite.texture;
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
	pbody = App->physics->CreateRectangle(sprite.pos.x, sprite.pos.y, width, height, 0.0f, cat, mask);
}

void Prefab::CreateCollision(int radius, int cat, int mask)
{
	pbody = App->physics->CreateCircle(sprite.pos.x, sprite.pos.y, radius, 0.0f, cat, mask);
}

void Prefab::CreateStaticCollision(int width, int height, int cat, int mask)
{
	pbody = App->physics->CreateStaticRectangle(sprite.pos.x, sprite.pos.y, width, height, 0.0f, cat, mask);
}

void Prefab::CreateStaticCollision(int radius, int cat, int mask)
{
	pbody = App->physics->CreateStaticCircle(sprite.pos.x, sprite.pos.y, radius, 0.0f, cat, mask);
}

iPoint Prefab::GetPosition() const
{
	iPoint pos;
	pbody->GetPosition(pos.x, pos.y);
	return pos;
}


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

void Prefab::CreateSensorCollision(int width, int height, int cat, int mask)
{
	pbody = App->physics->CreateRectangleSensor(sprite.pos.x, sprite.pos.y, width, height, 0.0f, cat, mask);
}

void Prefab::CreateSensorCollision(int radius, int cat, int mask)
{
	pbody = App->physics->CreateCircleSensor(sprite.pos.x, sprite.pos.y, radius, 0.0f, cat, mask);
}

iPoint Prefab::GetPosition() const
{
	iPoint pos;
	pbody->GetPosition(pos.x, pos.y);
	return pos;
}

//return the index that contains the "type" animation or -1 if not found
int Prefab::FindAnimation(AnimTypes type)
{
	int ret = -1;
	for (int i = 0; i < animations.count(); i++) {
		if (animations[i]->type == type) {
			ret = i;
			break;
		}
	}
	return ret;
}

void Prefab::LoadAnimations(pugi::xml_node & node)
{
	for (pugi::xml_node anim = node.child("anim"); anim != NULL; anim = anim.next_sibling("anim")) {
		p2List<SDL_Rect> anim_rects;
		float speed = anim.attribute("speed").as_float(1.0f);
		int type = anim.attribute("type").as_int(-1);
		for (pugi::xml_node frame = anim.child("frame"); frame != NULL; frame = frame.next_sibling("frame")) {
			SDL_Rect new_frame = { frame.attribute("x").as_int(0),frame.attribute("y").as_int(0),frame.attribute("w").as_int(0),frame.attribute("h").as_int(0) };
			anim_rects.add(new_frame);
		}
		if (type != -1)
			animations.add(new Animation(anim_rects, speed, static_cast<AnimTypes>(type)));
	}
}


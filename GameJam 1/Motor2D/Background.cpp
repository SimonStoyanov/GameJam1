#include "Background.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "j1Textures.h"

Background::Background() : j1Module()
{
	name.create("background");
}

Background::~Background()
{
}

bool Background::Awake(pugi::xml_node &)
{
	return true;
}

bool Background::CleanUp()
{
	return true;
}

// ------------------------------------
// ------------------------------------

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

#include "Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "Prefabs.h"

Player::Player() : j1Module()
{
	name.create("player");
}

Player::~Player()
{
}

bool Player::Awake(pugi::xml_node &)
{
	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::LoadTextures()
{
	player = new Prefab(1, 0, nullptr, NULLRECT);
}

void Player::CreateColliders()
{
	player->body = App->physics->CreateRectangle(player->sprite.pos.x, player->sprite.pos.y, 10, 30);
}

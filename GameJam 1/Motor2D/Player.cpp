#include "Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

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

void Player::CreateColliders()
{
}

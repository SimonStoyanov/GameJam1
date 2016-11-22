#include "Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "Prefabs.h"
#include "Dummy_Scene.h"

#define PLAYER_START_POSITION 10

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

bool Player::Update(float dt)
{
	int x; int y;
	player->pbody->body->SetTransform(b2Vec2(player->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(200*dt)), player->pbody->body->GetPosition().y),0);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && on_ground) {
		player->pbody->body->ApplyForceToCenter(b2Vec2(0, -2000), false);
		on_ground = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {

	}


	if (!player->pbody->body->IsAwake())
		player->pbody->body->SetAwake(true);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::LoadTextures()
{
	player = new Prefab(100, 0, nullptr, NULLRECT);
	player->CreateCollision(10, 30, PLAYER, WORLD);
	player->pbody->listener = this;
	player->pbody->body->SetBullet(true);
}

void Player::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyA == player->pbody) {
		if (App->scene->dummy_scene->IsGroundBody(bodyB)) {
			on_ground = true;
		}
	}
}



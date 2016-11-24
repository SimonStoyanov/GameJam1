#include "Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "Prefabs.h"
#include "Dummy_Scene.h"
#include "SpellManager.h"
#include "RandomGenerator.h"

Player::Player() : j1Module()
{
	name.create("player");
}

Player::~Player()
{
}

bool Player::Awake(pugi::xml_node & node)
{
	//Load Config from XML
	texture_path = node.child("spritesheet").attribute("path").as_string();
	Sprite_rect = { node.child("sprite").attribute("x").as_int(0), node.child("sprite").attribute("y").as_int(0), node.child("sprite").attribute("w").as_int(0), node.child("sprite").attribute("h").as_int(0) };
	jump_force = node.child("jump").attribute("force").as_int(10);
	start_pos.x = node.child("position").attribute("x").as_int(0);
	start_pos.y = node.child("position").attribute("y").as_int(0);

	return true;
}

bool Player::Start()
{
	App->spellmanager->Q = fireball;
	App->spellmanager->W = unknown;
	App->spellmanager->E = unknown;
	App->spellmanager->R = unknown;

	LoadTextures();

	last_pos = player->GetPosition().y;

	return true;
}

bool Player::Update(float dt)
{
	player->pbody->body->SetTransform(b2Vec2(player->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(200*dt)), player->pbody->body->GetPosition().y),0);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && on_ground) {
		player->pbody->body->ApplyForceToCenter(b2Vec2(0, -jump_force), false);
		on_ground = false;
	}

	if (!player->pbody->body->IsAwake())
		player->pbody->body->SetAwake(true);

	App->render->Blit(player->sprite.texture, player->GetPosition().x, player->GetPosition().y, &player->sprite.rect);

	// Platform shit ---------------------------------

	curr_platform = App->scene->dummy_scene->test_rand->GetClosestPlat();

	b2Filter a;
	
	if (player->GetPosition().y < 348 && !IsGoingUp()) //Ground
	{
		a.categoryBits = WORLD;
		a.maskBits = PLAYER;
		if(curr_platform != nullptr)
		curr_platform->body->GetFixtureList()->SetFilterData(a);
		LOG("CHANGING");
	}
		
	if (IsGoingUp())
	{
		a.categoryBits = PLAYER;
		a.maskBits = BOSS;
		player->pbody->body->GetFixtureList()->SetFilterData(a);
	}
	else
	{
		a.categoryBits = PLAYER;
		a.maskBits = WORLD;
		player->pbody->body->GetFixtureList()->SetFilterData(a);
	}
	
	// -------------------------------------------
	//LOG("%d", player->GetPosition().y);
	

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::LoadTextures()
{
	player = new Prefab(start_pos.x, start_pos.y, texture_path.GetString(), Sprite_rect);
	player->CreateCollision(Sprite_rect.w, Sprite_rect.h, PLAYER, WORLD);
	player->pbody->listener = this;
	player->pbody->body->SetBullet(true);
}

bool Player::IsGoingUp()
{
	bool ret = false;

	if(player->GetPosition().y < last_pos)
	{
		ret = true;
	}
	last_pos = player->GetPosition().y;
	return ret;
}

void Player::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyA == player->pbody) {
		if (bodyB->body->GetFixtureList()->GetFilterData().categoryBits == WORLD) 
		{
			on_ground = true;
		}

		if(bodyB->type == platform)
		{
			curr_platform = bodyB;
		}
	}
}





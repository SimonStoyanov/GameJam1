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
	//Load Player Config from XML
	pugi::xml_document playerconfig_doc;
	pugi::xml_node config_node;
	char* buf;
	int size = App->fs->Load("PlayerConfig.xml", &buf);
	playerconfig_doc.load_buffer(buf, size);
	RELEASE(buf);
	config_node = playerconfig_doc.child("config");

	draw_offset.x = config_node.child("draw_offset").attribute("x").as_int(0);
	draw_offset.y = config_node.child("draw_offset").attribute("y").as_int(0);

	App->spellmanager->Q = fireball;
	App->spellmanager->W = unknown;
	App->spellmanager->E = unknown;
	App->spellmanager->R = unknown;

	LoadTextures();
	player->LoadAnimations(config_node);
	current_animation = player->FindAnimation(Run);

	last_pos = player->GetPosition().y;

	return true;
}

bool Player::Update(float dt)
{
	if ((player->GetPosition().x + App->render->camera.x) < start_pos.x - 5) {
		player->pbody->body->SetTransform(b2Vec2(player->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(100 * dt)), player->pbody->body->GetPosition().y), 0);
	}
	if ((player->GetPosition().x + App->render->camera.x) > start_pos.x + 5) {
		player->pbody->body->SetTransform(b2Vec2(player->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(-100 * dt)), player->pbody->body->GetPosition().y), 0);
	}
	
	player->pbody->body->SetTransform(b2Vec2(player->pbody->body->GetPosition().x + PIXEL_TO_METERS((int)(200*dt)), player->pbody->body->GetPosition().y),0);

	if (player->pbody->body->GetLinearVelocity().x != 0) {
		player->pbody->body->SetLinearVelocity(b2Vec2(0, player->pbody->body->GetLinearVelocity().y));
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && on_ground) {
		player->pbody->body->ApplyForceToCenter(b2Vec2(0, -jump_force), false);
		current_animation = player->FindAnimation(Jump);
		on_ground = false;
	}
	else if (on_ground){
		current_animation = player->FindAnimation(Run);
	}

	if (!player->pbody->body->IsAwake())
		player->pbody->body->SetAwake(true);

	App->render->Blit(player->sprite.texture, player->GetPosition().x + draw_offset.x, player->GetPosition().y + draw_offset.y, &player->animations[current_animation]->GetCurrentFrameRect());

	// Random updater ---
	App->scene->dummy_scene->platforms_rand->CheckRand(-App->render->camera.x + 1000, App->player->player->GetPosition().y, 1500);

	// Platform shit ---------------------------------

	curr_platform = App->scene->dummy_scene->platforms_rand->GetClosestPlat();

	//LOG("%d", player->GetPosition().y);
	if (curr_platform != nullptr)
	{
		b2Filter a;
		if (player->GetPosition().y < 333 && !IsGoingUp()) //Ground
		{
			a.categoryBits = WORLD;
			a.maskBits = PLAYER;
			if (curr_platform != nullptr)
				curr_platform->body->GetFixtureList()->SetFilterData(a);
		}
		else
		{
			a.categoryBits = WORLD;
			a.maskBits = BOSS;
			if (curr_platform != nullptr)
				curr_platform->body->GetFixtureList()->SetFilterData(a);
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

		if (curr_platform->type == wall)
		{
			a.categoryBits = PLAYER;
			a.maskBits = WORLD;
			player->pbody->body->GetFixtureList()->SetFilterData(a);

			a.categoryBits = WORLD;
			a.maskBits = PLAYER;
			if (curr_platform != nullptr)
				curr_platform->body->GetFixtureList()->SetFilterData(a);
		}
	}
	
	// -------------------------------------------
	
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
	}
}





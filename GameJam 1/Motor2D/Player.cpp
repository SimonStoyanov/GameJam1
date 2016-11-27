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
#include "ModulePhysics.h"
#include "j1Timer.h"

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

	// Spells
	App->spellmanager->Q = fireball;
	App->spellmanager->W = shield;
	App->spellmanager->E = Spelltypes::ghost;
	App->spellmanager->R = firebarrage;


	LoadTextures();
	player->LoadAnimations(config_node.child("human"));
	current_animation = player->FindAnimation(Run);

	cat_anims = new Prefab(0, 0, "", NULLRECT);
	cat_anims->LoadAnimations(config_node.child("cat"));

	// Platforms
	last_pos = player->GetPosition().y;

	// UI
	UI_texture = App->tex->Load("Spritesheets/UI_sheet.png");
	UI_spells_rects.add({0, 0, 273, 92}); // bg
	UI_spells_rects.add({ 8, 104, 62, 76 }); // Q
	UI_spells_rects.add({ 75, 104, 62, 76 }); // W
	UI_spells_rects.add({ 138, 104, 62, 76 }); // E
	UI_spells_rects.add({ 202, 104, 62, 76 }); // R

	curr_hp = 3;

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

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && on_ground && !ghost) {
		player->pbody->body->ApplyForceToCenter(b2Vec2(0, -jump_force), false);
		current_animation = player->FindAnimation(Jump);
		on_ground = false;
	}
	else if (on_ground){
		current_animation = player->FindAnimation(Run);
	}

	if (!player->pbody->body->IsAwake())
		player->pbody->body->SetAwake(true);

	//Draw
	switch (shape)
	{
	case Human:
		App->render->Blit(player->sprite.texture, player->GetPosition().x + draw_offset.x, player->GetPosition().y + draw_offset.y, &player->animations[current_animation]->GetCurrentFrameRect());
		break;
	case Cat:
		App->render->Blit(player->sprite.texture, player->GetPosition().x + draw_offset.x, player->GetPosition().y + draw_offset.y-20, &cat_anims->animations[current_animation]->GetCurrentFrameRect());
		break;
	default:
		break;
	}
	
	//Return tu human shape
	if (shape != Human && shape_time.ReadSec() > 10)
		ChangeShape(Human);

	// Stop Ghost
	if (ghost && ghost_timer.ReadSec() > 5)
		UnGhost();

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
			if (!isTouching(player->pbody, curr_platform))
			{
				a.categoryBits = PLAYER;
				a.maskBits = WORLD;
				player->pbody->body->GetFixtureList()->SetFilterData(a);
			}
		}

		if (curr_platform->type == wall)
		{
			a.categoryBits = PLAYER;
			a.maskBits = WORLD;
			player->pbody->body->GetFixtureList()->SetFilterData(a);

			if(abs(DistanceToPlayer(curr_platform)) > 200)
			{
				a.categoryBits = PLAYER;
				a.maskBits = PLAYER;
				if (curr_platform != nullptr)
					curr_platform->body->GetFixtureList()->SetFilterData(a);
			}
			else
			{
				a.categoryBits = WORLD;
				a.maskBits = PLAYER;
				if (curr_platform != nullptr)
					curr_platform->body->GetFixtureList()->SetFilterData(a);
			}
		}
	}
	
	// -------------------------------------------
	return true;
}

bool Player::CleanUp()
{
	if (player != nullptr) {
		App->physics->DeleteObject(player->pbody);
		App->tex->UnLoad(player->sprite.texture);
	}
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

bool Player::IsGoingDown()
{
	bool ret = false;

	if (player->GetPosition().y > last_pos)
	{
		ret = true;
	}
	last_pos = player->GetPosition().y;
	return ret;
}

int Player::DistanceToPlayer(PhysBody* obj)
{
	int x; int y;
	obj->GetPosition(x, y);

	int dX0 = App->player->player->GetPosition().x;
	int dX1 = x;

	int dY0 = App->player->player->GetPosition().y;
	int dY1 = y;
	int dis = sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));

	return dis;
}

bool Player::isTouching(PhysBody * body1, PhysBody * body2)
{
	b2Body* b1 = body1->body;
	b2Body* b2 = body2->body;

	bool overlap = b2TestOverlap(b1->GetFixtureList()->GetShape(), 1, b2->GetFixtureList()->GetShape(), 1,  b1->GetTransform(), b2->GetTransform());
	return overlap;
}

void Player::ChangeShape(Shape newshape)
{
	shape = newshape;
	switch (shape)
	{
	case Human:
		App->spellmanager->Q = fireball;
		App->spellmanager->W = shield;
		App->spellmanager->E = Spelltypes::ghost;
		App->spellmanager->R = unknown;
		break;
	case Cat:
		App->spellmanager->Q = unknown;
		App->spellmanager->W = unknown;
		App->spellmanager->E = Spelltypes::ghost;
		App->spellmanager->R = unknown;
		break;
	default:
		break;
	}
	if (shape != Human)
		shape_time.Start();
}

void Player::Ghost()
{
	b2Filter a;
	a.categoryBits = WORLD;
	a.maskBits = PLAYER;
	player->pbody->body->GetFixtureList()->SetFilterData(a);
	ghost = true;
	player->pbody->body->SetGravityScale(0);
}

void Player::UnGhost()
{
	b2Filter a;
	a.categoryBits = PLAYER;
	a.maskBits = WORLD;
	player->pbody->body->GetFixtureList()->SetFilterData(a);
	ghost = false;
	player->pbody->body->SetGravityScale(1);
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





#include "Dummy_Scene.h"
#include "p2Defs.h"
#include "j1App.h"
#include "Player.h"
#include "Prefabs.h"
#include "ModulePhysics.h"
#include "j1Input.h"
#include "j1Text.h"

Dummy::Dummy() : Scene()
{
}

Dummy::~Dummy()
{
}

bool Dummy::Start()
{
	App->player->LoadTextures();
	App->player->CreateColliders();

	ground = new Prefab(430, 560, nullptr, NULLRECT);
	ground->CreateStaticCollision(870, 8, 5, 2);

	return true;
}

bool Dummy::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		int x; int y;
		App->player->player->body->GetPosition(x, y);
		LOG("%d %d", x, y);
		App->player->player->body->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x+0.0002f), PIXEL_TO_METERS(y)), App->player->player->body->GetRotation());
		App->player->player->body->GetPosition(x, y);
		LOG("%d %d", x, y);

	}

	return true;
}

void Dummy::Draw()
{
}

bool Dummy::CleanUp()
{
	if (ground != nullptr)
	{
		delete ground;
		ground = nullptr;
	}
	return true;
}

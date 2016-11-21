#include "Dummy_Scene.h"
#include "p2Defs.h"
#include "j1App.h"
#include "Player.h"
#include "Prefabs.h"
#include "ModulePhysics.h"

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

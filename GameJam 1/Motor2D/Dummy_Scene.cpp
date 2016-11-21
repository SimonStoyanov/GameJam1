#include "Dummy_Scene.h"
#include "j1App.h"
#include "Player.h"

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
	return true;
}

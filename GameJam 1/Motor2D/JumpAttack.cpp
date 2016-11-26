#include "JumpAttack.h"
#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include <math.h>
#include "SpellManager.h"

JumpAttack::JumpAttack(pugi::xml_node & config)
{
}

JumpAttack::~JumpAttack()
{
}

void JumpAttack::Start()
{
}

bool JumpAttack::Update()
{
	App->player->player->pbody->body->SetLinearVelocity(b2Vec2(0, 50));

	return true;
}

void JumpAttack::Draw()
{
}

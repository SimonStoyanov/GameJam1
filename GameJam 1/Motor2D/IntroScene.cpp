#include "Button.h"
#include "IntroScene.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"
#include "Player.h"
#include "j1App.h"

IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Start()
{
	play = new Button(345, 200, 150, 60);

	return true;
}

bool IntroScene::Update(float dt)
{
	if (play->MouseDown()) {
		App->player->Start();
		App->player->active = true;
		App->scene->ChangeScene(App->scene->dummy_scene);
	}
	return true;
}

bool IntroScene::PostUpdate()
{
	play->Draw();
	return true;
}

void IntroScene::Draw()
{
}

bool IntroScene::CleanUp()
{
	return true;
}

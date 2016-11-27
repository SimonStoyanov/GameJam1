#include "LoseScene.h"
#include "Button.h"
#include "IntroScene.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"
#include "Player.h"
#include "j1App.h"
#include "Prefabs.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include"j1Render.h"

LoseScene::LoseScene()
{
}

LoseScene::~LoseScene()
{
}

bool LoseScene::Start()
{
	pugi::xml_document Winconfig_doc;
	pugi::xml_node config_node;
	char* buf;
	int size = App->fs->Load("LoseScene.xml", &buf);
	Winconfig_doc.load_buffer(buf, size);
	RELEASE(buf);
	config_node = Winconfig_doc.child("config");

	pugi::xml_node buttons_node = config_node.child("buttons");

	SDL_Rect position;
	position.x = buttons_node.child("next").child("position").attribute("x").as_int(0);
	position.y = buttons_node.child("next").child("position").attribute("y").as_int(0);
	position.w = buttons_node.child("next").child("position").attribute("w").as_int(0);
	position.h = buttons_node.child("next").child("position").attribute("h").as_int(0);

	p2SString ui_path(buttons_node.child("image").attribute("path").as_string(nullptr));
	UI_tex = App->tex->Load(ui_path.GetString());

	p2SString background_path(config_node.child("background").child("image").attribute("path").as_string(nullptr));
	Background_tex = App->tex->Load(background_path.GetString());

	next = new Prefab(position.x, position.y, UI_tex, NULLRECT);
	next->LoadAnimations(buttons_node.child("next"));
	next->current_anim = next->FindAnimation(Idle);

	next_button = new Button(position.x, position.y, position.w, position.h);
	
	App->render->camera.x = 0;
	// Restart rounds
	App->scene->dummy_scene->round = 1;

	return true;
}

bool LoseScene::Update(float dt)
{
	if (next_button->MouseDown() && !next_clicked) {
		next->current_anim = next->FindAnimation(Run);
		next_clicked = true;
	}

	if (next_clicked) {
		if (true) {
			next_clicked = false;
			App->scene->ChangeScene(App->scene->intro_scene);
		}
	}
	return true;
}

bool LoseScene::PostUpdate()
{
	return true;
}

void LoseScene::Draw()
{
	next_button->Draw();
}

bool LoseScene::CleanUp()
{
	return true;
}
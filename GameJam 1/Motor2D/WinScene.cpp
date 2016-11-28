#include "WinScene.h"
#include "Button.h"
#include "IntroScene.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"
#include "Player.h"
#include "j1App.h"
#include "Prefabs.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Text.h"

WinScene::WinScene()
{
}

WinScene::~WinScene()
{
}

bool WinScene::Start()
{
	pugi::xml_document Winconfig_doc;
	pugi::xml_node config_node;
	char* buf;
	int size = App->fs->Load("WinScene.xml", &buf);
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

	position.x = buttons_node.child("exit").child("position").attribute("x").as_int(0);
	position.y = buttons_node.child("exit").child("position").attribute("y").as_int(0);
	position.w = buttons_node.child("exit").child("position").attribute("w").as_int(10);
	position.h = buttons_node.child("exit").child("position").attribute("h").as_int(10);

	exit = new Prefab(position.x, position.y, UI_tex, NULLRECT);
	exit->LoadAnimations(buttons_node.child("exit"));
	exit->current_anim = next->FindAnimation(Idle);

	exit_button = new Button(position.x, position.y, position.w, position.h);

	if (App->text->end_text == nullptr)
		App->text->end_text = new Text(200, 402, App->text->on_meth_30, 30);
	p2SString endtext("Congratulations! You Win!");
	App->text->end_text->SetText(endtext, middle);

	return true;
}

bool WinScene::Update(float dt)
{
	if (next_button->MouseDown() && !next_clicked) {
		next->current_anim = next->FindAnimation(Run);
		next_clicked = true;
	}

	if (next_clicked) {
		if (next->animations[next->current_anim]->Finished()) {
			next_clicked = false;
			App->player->Start();
			App->player->active = true;
			App->scene->ChangeScene(App->scene->dummy_scene);
		}
	}

	if (exit_button->MouseDown() && !exit_clicked) {
		exit->current_anim = exit->FindAnimation(Run);
		exit_clicked = true;
	}

	if (exit_clicked) {
		if (exit->animations[exit->current_anim]->Finished()) {
			exit_clicked = false;
			App->scene->ChangeScene(App->scene->intro_scene);
		}
	}

	return true;
}

bool WinScene::PostUpdate()
{
	App->text->end_text->PrintText();
	return false;
}

void WinScene::Draw()
{
	next_button->Draw();
	exit_button->Draw();
	if (Background_tex != nullptr)			//ALWAYS FIRST
		App->render->Blit(Background_tex, 0, 0);

	App->render->Blit(next->sprite.texture, next->sprite.pos.x, next->sprite.pos.y, &next->animations[next->current_anim]->GetCurrentFrameRect());
	App->render->Blit(exit->sprite.texture, exit->sprite.pos.x, exit->sprite.pos.y, &exit->animations[exit->current_anim]->GetCurrentFrameRect());
}

bool WinScene::CleanUp()
{
	return false;
}
